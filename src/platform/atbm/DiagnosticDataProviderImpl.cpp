/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Provides an implementation of the DiagnosticDataProvider object
 *          for ATBM platform.
 */

#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <app-common/zap-generated/enums.h>
#include <crypto/CHIPCryptoPAL.h>
#include <platform/DiagnosticDataProvider.h>
#include <platform/atbm/DiagnosticDataProviderImpl.h>
#include <platform/atbm/ATBMUtils.h>

#include "atbm_general.h"

using namespace ::chip;
using namespace ::chip::TLV;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceLayer::Internal;
using namespace ::chip::app::Clusters::GeneralDiagnostics;

namespace {

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
uint8_t MapAuthModeToSecurityType(int authmode)
{
    switch (authmode)
    {
    case KEY_NONE:
        return 1;
    case KEY_WEP:
    case KEY_WEP_SHARE:
        return 2;
    case KEY_WPA:
        return 3;
    case KEY_WPA2:
        return 4;
    case KEY_WPA3:
        return 5;
    default:
        return 0;
    }
}
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI

} // namespace

namespace chip {
namespace DeviceLayer {

DiagnosticDataProviderImpl & DiagnosticDataProviderImpl::GetDefaultInstance()
{
    static DiagnosticDataProviderImpl sInstance;
    return sInstance;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapFree(uint64_t & currentHeapFree)
{
    currentHeapFree = sys_mem_free_size_get();
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapUsed(uint64_t & currentHeapUsed)
{
    currentHeapUsed = sys_mem_total_size_get() - sys_mem_free_size_get();
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetCurrentHeapHighWatermark(uint64_t & currentHeapHighWatermark)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetRebootCount(uint16_t & rebootCount)
{
    uint32_t count = 0;

    CHIP_ERROR err = ConfigurationMgr().GetRebootCount(count);

    if (err == CHIP_NO_ERROR)
    {
        VerifyOrReturnError(count <= UINT16_MAX, CHIP_ERROR_INVALID_INTEGER_VALUE);
        rebootCount = static_cast<uint16_t>(count);
    }

    return err;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetUpTime(uint64_t & upTime)
{
    System::Clock::Timestamp currentTime = System::SystemClock().GetMonotonicTimestamp();
    System::Clock::Timestamp startTime   = PlatformMgrImpl().GetStartTime();

    if (currentTime >= startTime)
    {
        upTime = std::chrono::duration_cast<System::Clock::Seconds64>(currentTime - startTime).count();
        return CHIP_NO_ERROR;
    }

    return CHIP_ERROR_INVALID_TIME;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetTotalOperationalHours(uint32_t & totalOperationalHours)
{
    uint64_t upTime = 0;

    if (GetUpTime(upTime) == CHIP_NO_ERROR)
    {
        uint32_t totalHours = 0;
        if (ConfigurationMgr().GetTotalOperationalHours(totalHours) == CHIP_NO_ERROR)
        {
            VerifyOrReturnError(upTime / 3600 <= UINT32_MAX, CHIP_ERROR_INVALID_INTEGER_VALUE);
            totalOperationalHours = totalHours + static_cast<uint32_t>(upTime / 3600);
            return CHIP_NO_ERROR;
        }
    }

    return CHIP_ERROR_INVALID_TIME;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetBootReason(BootReasonType & bootReason)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetNetworkInterfaces(NetworkInterface ** netifpp)
{
    NetworkInterface * ifp = new NetworkInterface();
    struct netif * netif     = atbm_wifi_get_sta_netif();
    if (netif == NULL)
    {
        ChipLogError(DeviceLayer, "Failed to get network interfaces");
    }
    else
    {
        strcpy(ifp->Name, netif->name);
        ifp->name          = CharSpan::fromCharString(ifp->Name);
        ifp->isOperational = true;
        ifp->type          = EMBER_ZCL_INTERFACE_TYPE_WI_FI;
        ifp->offPremiseServicesReachableIPv4.SetNull();
        ifp->offPremiseServicesReachableIPv6.SetNull();
        atbm_wifi_get_mac_addr((u8 *)&ifp->MacAddress);
        ifp->hardwareAddress = ByteSpan(ifp->MacAddress, 6);

        memcpy(ifp->Ipv4AddressesBuffer[0], &(netif->ip_addr.u_addr.ip4.addr), kMaxIPv4AddrSize);
        ifp->Ipv4AddressSpans[0] = ByteSpan(ifp->Ipv4AddressesBuffer[0], kMaxIPv4AddrSize);
        ifp->IPv4Addresses       = chip::app::DataModel::List<chip::ByteSpan>(ifp->Ipv4AddressSpans, 1);

        uint8_t ipv6_addr_count = 0;
        ip6_addr_t ip6_addr[kMaxIPv6AddrCount];
        ipv6_addr_count = netif_get_ip6_addr_match(netif, ip6_addr);
        for (uint8_t idx = 0; idx < ipv6_addr_count; ++idx)
        {
            memcpy(ifp->Ipv6AddressesBuffer[idx], ip6_addr[idx].addr, kMaxIPv6AddrSize);
            ifp->Ipv6AddressSpans[idx] = ByteSpan(ifp->Ipv6AddressesBuffer[idx], kMaxIPv6AddrSize);
        }
        ifp->IPv6Addresses = chip::app::DataModel::List<chip::ByteSpan>(ifp->Ipv6AddressSpans, ipv6_addr_count);
    }
    *netifpp = ifp;

    return CHIP_NO_ERROR;
}

void DiagnosticDataProviderImpl::ReleaseNetworkInterfaces(NetworkInterface * netifp)
{
    while (netifp)
    {
        NetworkInterface * del = netifp;
        netifp                 = netifp->Next;
        delete del;
    }
}

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiBssId(ByteSpan & BssId)
{
    static uint8_t macAddress[kMaxHardwareAddrSize];
    u8 mac_addr[6];
    atbm_wifi_get_connected_destAddr((u8 *)&mac_addr);
    memcpy(macAddress, mac_addr, 6);
    BssId = ByteSpan(macAddress, 6);
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiSecurityType(uint8_t & securityType)
{
    struct atbmwifi_configure config;
    atbm_wifi_get_config(&config);
    securityType = MapAuthModeToSecurityType(config.key_mgmt);
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiVersion(uint8_t & wifiVersion)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiChannelNumber(uint16_t & channelNumber)
{
    channelNumber = atbm_wifi_get_channel();
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiRssi(int8_t & rssi)
{
    rssi = atbm_wifi_get_rssi();
    return CHIP_NO_ERROR;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiBeaconLostCount(uint32_t & beaconLostCount)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiCurrentMaxRate(uint64_t & currentMaxRate)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiPacketMulticastRxCount(uint32_t & packetMulticastRxCount)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiPacketMulticastTxCount(uint32_t & packetMulticastTxCount)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiPacketUnicastRxCount(uint32_t & packetUnicastRxCount)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiPacketUnicastTxCount(uint32_t & packetUnicastTxCount)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::GetWiFiOverrunCount(uint64_t & overrunCount)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR DiagnosticDataProviderImpl::ResetWiFiNetworkDiagnosticsCounts()
{
    return CHIP_NO_ERROR;
}
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI

DiagnosticDataProvider & GetDiagnosticDataProviderImpl()
{
    return DiagnosticDataProviderImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip
