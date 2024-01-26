/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2018 Nest Labs, Inc.
 *    All rights reserved.
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
 *          General utility methods for the ATBM platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <lib/support/CodeUtils.h>
#include <lib/support/ErrorStr.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/atbm/ATBMUtils.h>

#include "atbm_general.h"

using namespace ::chip::DeviceLayer::Internal;
using chip::DeviceLayer::Internal::DeviceNetworkInfo;

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP
CHIP_ERROR ATBMUtils::IsAPEnabled(bool & apEnabled)
{
    apEnabled = atbm_wifi_is_iftype_ap();

    return CHIP_NO_ERROR;
}
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP

CHIP_ERROR ATBMUtils::IsStationEnabled(bool & staEnabled)
{
    staEnabled = atbm_wifi_is_iftype_station();

    return CHIP_NO_ERROR;
}

bool ATBMUtils::IsStationProvisioned(void)
{
    struct atbmwifi_configure stationConfig;

    atbm_wifi_get_config(&stationConfig);
    return (stationConfig.ssid[0] != 0);
}

CHIP_ERROR ATBMUtils::IsStationConnected(bool & connected)
{
    connected = atbm_wifi_is_connect_ok();
    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMUtils::StartWiFiLayer(void)
{
    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMUtils::EnableStationMode(void)
{
    if (atbm_wifi_is_iftype_station())
    {
        ChipLogProgress(DeviceLayer, "ATBM WiFi mode is STA");
        return CHIP_NO_ERROR;
    }

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP
    // If station mode is not already enabled (implying the current mode is WIFI_MODE_AP), change
    // the mode to WIFI_MODE_APSTA.
    if (atbm_wifi_is_iftype_ap())
    {
        ChipLogProgress(DeviceLayer, "Changing ATBM WiFi mode: AP -> STA");

        atbm_wifi_start_station_call_task();

    }
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP

    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMUtils::SetAPMode(bool enabled)
{
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP
    if (!enabled && atbm_wifi_is_iftype_ap())
    {
        atbm_wifi_stopap_call_task();
    }
    else if (enabled && !atbm_wifi_is_iftype_ap())
    {
        atbm_wifi_startap_call_task();
    }
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP

    return CHIP_NO_ERROR;
}

struct netif * ATBMUtils::GetStationNetif(void)
{
    return atbm_wifi_get_sta_netif();
}

struct netif * ATBMUtils::GetNetif(const char * ifKey)
{
    return atbm_wifi_get_sta_netif();
}

bool ATBMUtils::IsInterfaceUp(const char * ifKey)
{
    struct netif * netif = GetNetif(ifKey);
    return netif != NULL && netif_is_up(netif);
}

bool ATBMUtils::HasIPv6LinkLocalAddress(const char * ifKey)
{
    return netif_ip6_addr_state(atbm_wifi_get_sta_netif(), 0) & BIT(IP6_ADDR_INVALID);
}

CHIP_ERROR ATBMUtils::GetWiFiStationProvision(Internal::DeviceNetworkInfo & netInfo, bool includeCredentials)
{
    struct atbmwifi_configure stationConfig;

    atbm_wifi_get_config(&stationConfig);

    VerifyOrReturnError(stationConfig.ssid[0] != 0, CHIP_ERROR_INCORRECT_STATE);

    netInfo.NetworkId              = kWiFiStationNetworkId;
    netInfo.FieldPresent.NetworkId = true;
    memcpy(netInfo.WiFiSSID, stationConfig.ssid,
           min(strlen(reinterpret_cast<char *>(stationConfig.ssid)) + 1, sizeof(netInfo.WiFiSSID)));

    // Enforce that netInfo wifiSSID is null terminated
    netInfo.WiFiSSID[kMaxWiFiSSIDLength] = '\0';

    if (includeCredentials)
    {
        static_assert(sizeof(netInfo.WiFiKey) < 255, "Our min might not fit in netInfo.WiFiKeyLen");
        netInfo.WiFiKeyLen = static_cast<uint8_t>(min(strlen((char *) stationConfig.password), sizeof(netInfo.WiFiKey)));
        memcpy(netInfo.WiFiKey, stationConfig.password, netInfo.WiFiKeyLen);
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMUtils::SetWiFiStationProvision(const Internal::DeviceNetworkInfo & netInfo)
{
    char wifiSSID[kMaxWiFiSSIDLength + 1];
    size_t netInfoSSIDLen = strlen(netInfo.WiFiSSID);

    // Enforce that wifiSSID is null terminated before copying it
    memcpy(wifiSSID, netInfo.WiFiSSID, min(netInfoSSIDLen + 1, sizeof(wifiSSID)));
    if (netInfoSSIDLen + 1 < sizeof(wifiSSID))
    {
        wifiSSID[netInfoSSIDLen] = '\0';
    }
    else
    {
        wifiSSID[kMaxWiFiSSIDLength] = '\0';
    }

    atbm_wifi_set_config((u8 *)wifiSSID, strlen(wifiSSID), (u8 *)netInfo.WiFiKey, netInfo.WiFiKeyLen, KEY_WPA2, 0, NULL);

    ReturnErrorOnFailure(ATBMUtils::EnableStationMode());

    ChipLogProgress(DeviceLayer, "WiFi station provision set (SSID: %s)", netInfo.WiFiSSID);

    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMUtils::ClearWiFiStationProvision(void)
{
    // Clear the ATBM WiFi station configuration.
    atbm_wifi_clear_config();

    return CHIP_NO_ERROR;
}

