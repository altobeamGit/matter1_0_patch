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
 *          Provides an implementation of the PlatformManager object
 *          for the ATBM platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <app-common/zap-generated/enums.h>
#include <crypto/CHIPCryptoPAL.h>
#include <platform/atbm/DiagnosticDataProviderImpl.h>
#include <platform/atbm/ATBMUtils.h>
#include <platform/atbm/SystemTimeSupport.h>
#include <platform/PlatformManager.h>
#include <platform/internal/GenericPlatformManagerImpl_FreeRTOS.ipp>

namespace chip {
namespace DeviceLayer {

namespace Internal {
extern CHIP_ERROR InitLwIPCoreLock(void);
}

PlatformManagerImpl PlatformManagerImpl::sInstance;

static int app_entropy_source(void * data, unsigned char * output, size_t len, size_t * olen)
{
    random_get_bytes(output, len);
    *olen = len;
    return 0;
}

CHIP_ERROR PlatformManagerImpl::_InitChipStack(void)
{
    // Make sure the LwIP core lock has been initialized
    ReturnErrorOnFailure(Internal::InitLwIPCoreLock());

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
    {
        //register event
        atbm_wifi_matter_event_handler_register(PlatformManagerImpl::HandleATBMSystemEvent);
        mStartTime = System::SystemClock().GetMonotonicTimestamp();
        //wifi init
    }
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI

    ReturnErrorOnFailure(chip::Crypto::add_entropy_source(app_entropy_source, NULL, 16));

    // Call _InitChipStack() on the generic implementation base class
    // to finish the initialization process.
    ReturnErrorOnFailure(Internal::GenericPlatformManagerImpl_FreeRTOS<PlatformManagerImpl>::_InitChipStack());

    ReturnErrorOnFailure(System::Clock::InitClock_RealTime());

    return CHIP_NO_ERROR;
}

void PlatformManagerImpl::_Shutdown()
{
    uint64_t upTime = 0;

    if (GetDiagnosticDataProvider().GetUpTime(upTime) == CHIP_NO_ERROR)
    {
        uint32_t totalOperationalHours = 0;

        if (ConfigurationMgr().GetTotalOperationalHours(totalOperationalHours) == CHIP_NO_ERROR)
        {
            ConfigurationMgr().StoreTotalOperationalHours(totalOperationalHours + static_cast<uint32_t>(upTime / 3600));
        }
        else
        {
            ChipLogError(DeviceLayer, "Failed to get total operational hours of the Node");
        }
    }
    else
    {
        ChipLogError(DeviceLayer, "Failed to get current uptime since the Node’s last reboot");
    }

    Internal::GenericPlatformManagerImpl_FreeRTOS<PlatformManagerImpl>::_Shutdown();
}

void PlatformManagerImpl::HandleATBMSystemEvent(unsigned int eventId)//(int32_t eventId, void * eventData)
{
    ChipDeviceEvent event;
    memset(&event, 0, sizeof(event));
    event.Type                         = DeviceEventType::kATBMSystemEvent;
    event.Platform.ATBMSystemEvent.Id   = eventId;

    switch (eventId)
    {
    case IP_EVENT_STA_GOT_IP:
        //memcpy(&event.Platform.ATBMSystemEvent.Data.IpGotIp, eventData, sizeof(event.Platform.ATBMSystemEvent.Data.IpGotIp));
        break;
    case IP_EVENT_GOT_IP6:
        //memcpy(&event.Platform.ATBMSystemEvent.Data.IpGotIp6, eventData, sizeof(event.Platform.ATBMSystemEvent.Data.IpGotIp6));
        break;
    case WIFI_EVENT_SCAN_DONE:
        //memcpy(&event.Platform.ATBMSystemEvent.Data.WiFiStaScanDone, eventData,
        //       sizeof(event.Platform.ATBMSystemEvent.Data.WiFiStaScanDone));
        break;
    case WIFI_EVENT_STA_CONNECTED:
        //memcpy(&event.Platform.ATBMSystemEvent.Data.WiFiStaConnected, eventData,
        //       sizeof(event.Platform.ATBMSystemEvent.Data.WiFiStaConnected));
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        //memcpy(&event.Platform.ATBMSystemEvent.Data.WiFiStaDisconnected, eventData,
        //       sizeof(event.Platform.ATBMSystemEvent.Data.WiFiStaDisconnected));
        break;
    default:
        break;
    }

    sInstance.PostEventOrDie(&event);
}

} // namespace DeviceLayer
} // namespace chip
