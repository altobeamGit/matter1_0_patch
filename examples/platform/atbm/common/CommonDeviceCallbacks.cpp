/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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
#include "CommonDeviceCallbacks.h"

#include "route_hook/atbm_route_hook.h"
#include <app-common/zap-generated/attribute-id.h>
#include <app-common/zap-generated/cluster-id.h>
#include <app/server/Dnssd.h>
#include <app/util/util.h>
#include <lib/support/CodeUtils.h>
#if CONFIG_ENABLE_OTA_REQUESTOR
#include <ota/OTAHelper.h>
#endif

#include "atbm_general.h"

using namespace chip;
using namespace chip::DeviceLayer;
using namespace chip::System;

DeviceCallbacksDelegate * appDelegate = nullptr;
#if CONFIG_ENABLE_OTA_REQUESTOR
static bool isOTAInitialized = false;
#endif

void CommonDeviceCallbacks::DeviceEventCallback(const ChipDeviceEvent * event, intptr_t arg)
{
    switch (event->Type)
    {
    case DeviceEventType::kInternetConnectivityChange:
        ChipLogProgress(DeviceLayer, "Internet Connectivity Change");
        OnInternetConnectivityChange(event);
        break;

    case DeviceEventType::kCHIPoBLEConnectionEstablished:
        ChipLogProgress(DeviceLayer, "CHIPoBLE connection established");
        break;

    case DeviceEventType::kCHIPoBLEConnectionClosed:
        ChipLogProgress(DeviceLayer, "CHIPoBLE disconnected");
        break;

    case DeviceEventType::kThreadConnectivityChange:
#if CONFIG_ENABLE_OTA_REQUESTOR
        if (event->ThreadConnectivityChange.Result == kConnectivity_Established && !isOTAInitialized)
        {
            OTAHelpers::Instance().InitOTARequestor();
            isOTAInitialized = true;
        }
#endif
        break;

    case DeviceEventType::kCommissioningComplete: {
        ChipLogProgress(DeviceLayer, "Commissioning complete");
    }
    break;

    case DeviceEventType::kInterfaceIpAddressChanged:
        ChipLogProgress(DeviceLayer, "Interface IpAddress Changed");
        if ((event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV4_Assigned) ||
            (event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV6_Assigned))
        {
            // MDNS server restart on any ip assignment: if link local ipv6 is configured, that
            // will not trigger a 'internet connectivity change' as there is no internet
            // connectivity. MDNS still wants to refresh its listening interfaces to include the
            // newly selected address.
            ChipLogProgress(DeviceLayer, "app::DnssdServer Start");
            chip::app::DnssdServer::Instance().StartServer();
        }
        if (event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV6_Assigned)
        {
            ChipLogProgress(DeviceLayer, "start atbm_route_hook_init");
            if (atbm_route_hook_init())
            {
                ChipLogError(DeviceLayer, "atbm_route_hook_init fail");
            }
        }
        break;
    }

    //ChipLogProgress(DeviceLayer, "Current free heap: %u\n", static_cast<unsigned int>(sys_mem_free_size_get()));
}

void CommonDeviceCallbacks::OnInternetConnectivityChange(const ChipDeviceEvent * event)
{
    appDelegate = DeviceCallbacksDelegate::Instance().GetAppDelegate();
    if (event->InternetConnectivityChange.IPv4 == kConnectivity_Established)
    {
        ChipLogProgress(DeviceLayer, "IPv4 Server ready...");
        if (appDelegate != nullptr)
        {
            appDelegate->OnIPv4ConnectivityEstablished();
        }
        chip::app::DnssdServer::Instance().StartServer();
#if CONFIG_ENABLE_OTA_REQUESTOR
        if (!isOTAInitialized)
        {
            OTAHelpers::Instance().InitOTARequestor();
            isOTAInitialized = true;
        }
#endif
    }
    else if (event->InternetConnectivityChange.IPv4 == kConnectivity_Lost)
    {
        ChipLogProgress(DeviceLayer, "Lost IPv4 connectivity...");
        if (appDelegate != nullptr)
        {
            appDelegate->OnIPv4ConnectivityLost();
        }
    }
    if (event->InternetConnectivityChange.IPv6 == kConnectivity_Established)
    {
        ChipLogProgress(DeviceLayer, "IPv6 Server ready...");
        chip::app::DnssdServer::Instance().StartServer();

#if CONFIG_ENABLE_OTA_REQUESTOR
        if (!isOTAInitialized)
        {
            OTAHelpers::Instance().InitOTARequestor();
            isOTAInitialized = true;
        }
#endif
    }
    else if (event->InternetConnectivityChange.IPv6 == kConnectivity_Lost)
    {
        ChipLogProgress(DeviceLayer, "Lost IPv6 connectivity...");
    }
}
