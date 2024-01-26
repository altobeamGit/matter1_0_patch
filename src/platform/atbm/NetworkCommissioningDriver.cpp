/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include <lib/support/CodeUtils.h>
#include <lib/support/SafeInt.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/atbm/ATBMUtils.h>
#include <platform/atbm/NetworkCommissioningDriver.h>

#include "atbm_general.h"

#include <limits>
#include <string>

using namespace ::chip;
using namespace ::chip::DeviceLayer::Internal;
namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

namespace {
constexpr char kWiFiSSIDKeyName[]        = "wifi-ssid";
constexpr char kWiFiCredentialsKeyName[] = "wifi-pass";
static uint8_t WiFiSSIDStr[DeviceLayer::Internal::kMaxWiFiSSIDLength];
} // namespace

CHIP_ERROR ATBMWiFiDriver::Init(NetworkStatusChangeCallback * networkStatusChangeCallback)
{
    CHIP_ERROR err;
    size_t ssidLen        = 0;
    size_t credentialsLen = 0;

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiCredentialsKeyName, mSavedNetwork.credentials,
                                                   sizeof(mSavedNetwork.credentials), &credentialsLen);
    if (err == CHIP_ERROR_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiSSIDKeyName, mSavedNetwork.ssid, sizeof(mSavedNetwork.ssid), &ssidLen);
    if (err == CHIP_ERROR_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }
    mSavedNetwork.credentialsLen = credentialsLen;
    mSavedNetwork.ssidLen        = ssidLen;

    mStagingNetwork        = mSavedNetwork;
    mpScanCallback         = nullptr;
    mpConnectCallback      = nullptr;
    mpStatusChangeCallback = networkStatusChangeCallback;
    return err;
}

void ATBMWiFiDriver::Shutdown()
{
    mpStatusChangeCallback = nullptr;
}

CHIP_ERROR ATBMWiFiDriver::CommitConfiguration()
{
    ReturnErrorOnFailure(PersistedStorage::KeyValueStoreMgr().Put(kWiFiSSIDKeyName, mStagingNetwork.ssid, mStagingNetwork.ssidLen));
    ReturnErrorOnFailure(PersistedStorage::KeyValueStoreMgr().Put(kWiFiCredentialsKeyName, mStagingNetwork.credentials,
                                                                  mStagingNetwork.credentialsLen));
    mSavedNetwork = mStagingNetwork;
    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMWiFiDriver::RevertConfiguration()
{
    mStagingNetwork = mSavedNetwork;
    return CHIP_NO_ERROR;
}

bool ATBMWiFiDriver::NetworkMatch(const WiFiNetwork & network, ByteSpan networkId)
{
    return networkId.size() == network.ssidLen && memcmp(networkId.data(), network.ssid, network.ssidLen) == 0;
}

Status ATBMWiFiDriver::AddOrUpdateNetwork(ByteSpan ssid, ByteSpan credentials, MutableCharSpan & outDebugText,
                                         uint8_t & outNetworkIndex)
{
    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(mStagingNetwork.ssidLen == 0 || NetworkMatch(mStagingNetwork, ssid), Status::kBoundsExceeded);
    VerifyOrReturnError(credentials.size() <= sizeof(mStagingNetwork.credentials), Status::kOutOfRange);
    VerifyOrReturnError(ssid.size() <= sizeof(mStagingNetwork.ssid), Status::kOutOfRange);

    memcpy(mStagingNetwork.credentials, credentials.data(), credentials.size());
    mStagingNetwork.credentialsLen = static_cast<decltype(mStagingNetwork.credentialsLen)>(credentials.size());

    memcpy(mStagingNetwork.ssid, ssid.data(), ssid.size());
    mStagingNetwork.ssidLen = static_cast<decltype(mStagingNetwork.ssidLen)>(ssid.size());

    return Status::kSuccess;
}

Status ATBMWiFiDriver::RemoveNetwork(ByteSpan networkId, MutableCharSpan & outDebugText, uint8_t & outNetworkIndex)
{
    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);

    // Use empty ssid for representing invalid network
    mStagingNetwork.ssidLen = 0;
    return Status::kSuccess;
}

Status ATBMWiFiDriver::ReorderNetwork(ByteSpan networkId, uint8_t index, MutableCharSpan & outDebugText)
{
    outDebugText.reduce_size(0);

    // Only one network is supported now
    VerifyOrReturnError(index == 0, Status::kOutOfRange);
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);
    return Status::kSuccess;
}

CHIP_ERROR ATBMWiFiDriver::ConnectWiFiNetwork(const char * ssid, uint8_t ssidLen, const char * key, uint8_t keyLen)
{
    // If device is already connected to WiFi, then disconnect the WiFi,
    // clear the WiFi configurations and add the newly provided WiFi configurations.
    if (chip::DeviceLayer::Internal::ATBMUtils::IsStationProvisioned())
    {
        ChipLogProgress(DeviceLayer, "Disconnecting WiFi station interface");
        atbm_wifi_disconnect_call_task();

        CHIP_ERROR error = chip::DeviceLayer::Internal::ATBMUtils::ClearWiFiStationProvision();
        if (error != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ClearWiFiStationProvision failed: %s", chip::ErrorStr(error));
            return error;
        }
    }

    ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));

    atbm_wifi_set_config((u8 *)ssid, ssidLen, (u8 *)key, keyLen, KEY_WPA2, 0, NULL);

    ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));
    return ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled);
}

void ATBMWiFiDriver::OnConnectWiFiNetwork()
{
    if (mpConnectCallback)
    {
        DeviceLayer::SystemLayer().CancelTimer(OnConnectWiFiNetworkFailed, NULL);
        mpConnectCallback->OnResult(Status::kSuccess, CharSpan(), 0);
        mpConnectCallback = nullptr;
    }
}

void ATBMWiFiDriver::OnConnectWiFiNetworkFailed()
{
    if (mpConnectCallback)
    {
        mpConnectCallback->OnResult(Status::kNetworkNotFound, CharSpan(), 0);
        mpConnectCallback = nullptr;
    }
}

void ATBMWiFiDriver::OnConnectWiFiNetworkFailed(chip::System::Layer * aLayer, void * aAppState)
{
    CHIP_ERROR error = chip::DeviceLayer::Internal::ATBMUtils::ClearWiFiStationProvision();
    if (error != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ClearWiFiStationProvision failed: %s", chip::ErrorStr(error));
    }
    ATBMWiFiDriver::GetInstance().OnConnectWiFiNetworkFailed();
}

void ATBMWiFiDriver::ConnectNetwork(ByteSpan networkId, ConnectCallback * callback)
{
    CHIP_ERROR err          = CHIP_NO_ERROR;
    Status networkingStatus = Status::kSuccess;
    const uint32_t secToMiliSec = 1000;

    VerifyOrExit(NetworkMatch(mStagingNetwork, networkId), networkingStatus = Status::kNetworkIDNotFound);
    VerifyOrExit(mpConnectCallback == nullptr, networkingStatus = Status::kUnknownError);
    ChipLogProgress(NetworkProvisioning, "ATBM NetworkCommissioningDelegate: SSID: %.*s", static_cast<int>(networkId.size()),
                    networkId.data());

    err = ConnectWiFiNetwork(reinterpret_cast<const char *>(mStagingNetwork.ssid), mStagingNetwork.ssidLen,
                             reinterpret_cast<const char *>(mStagingNetwork.credentials), mStagingNetwork.credentialsLen);

    err = DeviceLayer::SystemLayer().StartTimer(
        static_cast<System::Clock::Timeout>(kWiFiConnectNetworkTimeoutSeconds * secToMiliSec), OnConnectWiFiNetworkFailed, NULL);
    mpConnectCallback = callback;
exit:
    if (err != CHIP_NO_ERROR)
    {
        networkingStatus = Status::kUnknownError;
    }
    if (networkingStatus != Status::kSuccess)
    {
        ChipLogError(NetworkProvisioning, "Failed to connect to WiFi network:%s", chip::ErrorStr(err));
        mpConnectCallback = nullptr;
        callback->OnResult(networkingStatus, CharSpan(), 0);
    }
}

CHIP_ERROR ATBMWiFiDriver::StartScanWiFiNetworks(ByteSpan ssid)
{
    int err = 0;

    ChipLogProgress(NetworkProvisioning, "ATBM Scan ALL...");
    err = atbm_wifi_scan();
    if (err != 0)
    {
        return CHIP_ERROR_INTERNAL;
    }
    return CHIP_NO_ERROR;
}

void ATBMWiFiDriver::OnScanWiFiNetworkDone()
{
    int ap_number;

    ap_number = atbm_wifi_scan_is_done();
    if (!ap_number || ap_number<0)
    {
        ChipLogProgress(DeviceLayer, "Scanning or No AP found");
        if (mpScanCallback != nullptr)
        {
            mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), nullptr);
            mpScanCallback = nullptr;
        }
        return;
    }

    struct atbmwifi_scan_result *scan_result = atbm_wifi_scan_get_result();
    if (scan_result == NULL)
    {
        ChipLogError(DeviceLayer, "can't get memory for ap_list_buffer");
        if (mpScanCallback)
        {
            mpScanCallback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
            mpScanCallback = nullptr;
        }
        return;
    }

    if (CHIP_NO_ERROR == DeviceLayer::SystemLayer().ScheduleLambda([ap_number, scan_result]() {
            ATBMScanResponseIterator iter(ap_number, scan_result);
            if (GetInstance().mpScanCallback)
            {
                GetInstance().mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), &iter);
                GetInstance().mpScanCallback = nullptr;
            }
            else
            {
                ChipLogError(DeviceLayer, "can't find the ScanCallback function");
            }
        }))
    {
        atbm_wifi_process_scan_result(NULL);
    }
    else
    {
        ChipLogError(DeviceLayer, "can't get ap_records ");
        if (mpScanCallback)
        {
            mpScanCallback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
            mpScanCallback = nullptr;
        }
    }
}

CHIP_ERROR GetConfiguredNetwork(Network & network)
{
    uint8_t ssid[64];
    uint16_t ssid_len;
    struct atbmwifi_configure config;

	atbm_wifi_get_config(&config);
	memcpy(&ssid, config.ssid, config.ssid_len);
    ssid_len = config.ssid_len;
    if (!ssid_len || ssid_len > DeviceLayer::Internal::kMaxWiFiSSIDLength)
    {
        return CHIP_ERROR_INTERNAL;
    }

    memcpy(network.networkID, ssid, ssid_len);
    network.networkIDLen = ssid_len;

    return CHIP_NO_ERROR;
}

void ATBMWiFiDriver::OnNetworkStatusChange()
{
    Network configuredNetwork;
    bool staEnabled = false, staConnected = false;
    VerifyOrReturn(ATBMUtils::IsStationEnabled(staEnabled) == CHIP_NO_ERROR);
    VerifyOrReturn(staEnabled && mpStatusChangeCallback != nullptr);
    CHIP_ERROR err = GetConfiguredNetwork(configuredNetwork);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to get configured network when updating network status: %s", err.AsString());
        return;
    }
    VerifyOrReturn(ATBMUtils::IsStationConnected(staConnected) == CHIP_NO_ERROR);
    if (staConnected)
    {
        mpStatusChangeCallback->OnNetworkingStatusChange(
            Status::kSuccess, MakeOptional(ByteSpan(configuredNetwork.networkID, configuredNetwork.networkIDLen)), NullOptional);
        return;
    }
    mpStatusChangeCallback->OnNetworkingStatusChange(
        Status::kUnknownError, MakeOptional(ByteSpan(configuredNetwork.networkID, configuredNetwork.networkIDLen)),
        MakeOptional(GetLastDisconnectReason()));
}

void ATBMWiFiDriver::ScanNetworks(ByteSpan ssid, WiFiDriver::ScanCallback * callback)
{
    if (callback != nullptr)
    {
        mpScanCallback = callback;
        if (StartScanWiFiNetworks(ssid) != CHIP_NO_ERROR)
        {
            mpScanCallback = nullptr;
            callback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
        }
    }
}

CHIP_ERROR ATBMWiFiDriver::SetLastDisconnectReason(const ChipDeviceEvent * event)
{
    VerifyOrReturnError(event->Type == DeviceEventType::kATBMSystemEvent &&
                            event->Platform.ATBMSystemEvent.Id == WIFI_EVENT_STA_DISCONNECTED,
                        CHIP_ERROR_INVALID_ARGUMENT);
    mLastDisconnectedReason = atbm_wifi_get_disconn_reason_code();
    return CHIP_NO_ERROR;
}

int32_t ATBMWiFiDriver::GetLastDisconnectReason()
{
    return mLastDisconnectedReason;
}

size_t ATBMWiFiDriver::WiFiNetworkIterator::Count()
{
    return mDriver->mStagingNetwork.ssidLen == 0 ? 0 : 1;
}

bool ATBMWiFiDriver::WiFiNetworkIterator::Next(Network & item)
{
    if (mExhausted || mDriver->mStagingNetwork.ssidLen == 0)
    {
        return false;
    }
    memcpy(item.networkID, mDriver->mStagingNetwork.ssid, mDriver->mStagingNetwork.ssidLen);
    item.networkIDLen = mDriver->mStagingNetwork.ssidLen;
    item.connected    = false;
    mExhausted        = true;

    Network configuredNetwork;
    CHIP_ERROR err = GetConfiguredNetwork(configuredNetwork);
    if (err == CHIP_NO_ERROR)
    {
        bool isConnected = false;
        err              = ATBMUtils::IsStationConnected(isConnected);
        if (err == CHIP_NO_ERROR && isConnected && configuredNetwork.networkIDLen == item.networkIDLen &&
            memcmp(configuredNetwork.networkID, item.networkID, item.networkIDLen) == 0)
        {
            item.connected = true;
        }
    }
    return true;
}

} // namespace NetworkCommissioning
} // namespace DeviceLayer
} // namespace chip
