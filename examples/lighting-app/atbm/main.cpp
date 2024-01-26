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

#include "DeviceCallbacks.h"

#include "AppTask.h"
#include <common/CHIPDeviceManager.h>
#include <common/ATBMAppServer.h>

#include <app/server/Dnssd.h>
#include <app/server/OnboardingCodesUtil.h>
#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#if CONFIG_ENABLE_ATBM_FACTORY_DATA_PROVIDER
#include <platform/atbm/ATBMFactoryDataProvider.h>
#else
#include "CommissionableInit.h"
#endif // CONFIG_ENABLE_ATBM_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_ATBM_DEVICE_INFO_PROVIDER
#include <platform/atbm/ATBMDeviceInfoProvider.h>
#else
#include <DeviceInfoProviderImpl.h>
#endif // CONFIG_ENABLE_ATBM_DEVICE_INFO_PROVIDER

#include "launch_shell.h"
#include "atbm_general.h"
#include "easyflash.h"

using namespace ::chip;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceManager;
using namespace ::chip::DeviceLayer;

static AppDeviceCallbacks EchoCallbacks;

namespace {
#if CONFIG_ENABLE_ATBM_FACTORY_DATA_PROVIDER
DeviceLayer::ATBMFactoryDataProvider sFactoryDataProvider;
#else
ExampleCommissionableDataProvider gExampleCommissionableDataProvider;
#endif // CONFIG_ENABLE_ATBM_FACTORY_DATA_PROVIDER

#if CONFIG_ENABLE_ATBM_DEVICE_INFO_PROVIDER
DeviceLayer::ATBMDeviceInfoProvider gExampleDeviceInfoProvider;
#else
DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
#endif // CONFIG_ENABLE_ATBM_DEVICE_INFO_PROVIDER
} // namespace

static void InitServer(intptr_t context)
{
    ATBMAppServer::Init();
}

extern "C" void app_matter_main()
{
    ChipLogProgress(DeviceLayer, "Enter app_matter_main...");
    //platform init
    EfErrCode err = easyflash_init();
    if (err != EF_NO_ERR)
    {
        ChipLogError(DeviceLayer, "easyflash_init fail...");
        return;
    }

    chip::LaunchShell();

    DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    CHIPDeviceManager & deviceMgr = CHIPDeviceManager::GetInstance();
    CHIP_ERROR error              = deviceMgr.Init(&EchoCallbacks);
    if (error != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "device.Init() failed: %s", ErrorStr(error));
        return;
    }

#if CONFIG_ENABLE_ATBM_FACTORY_DATA_PROVIDER
    SetCommissionableDataProvider(&sFactoryDataProvider);
    SetDeviceAttestationCredentialsProvider(&sFactoryDataProvider);
#if CONFIG_ENABLE_ATBM_DEVICE_INSTANCE_INFO_PROVIDER
    SetDeviceInstanceInfoProvider(&sFactoryDataProvider);
#endif
#else
    error = chip::examples::InitCommissionableDataProvider(gExampleCommissionableDataProvider);
    if (error != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "InitCommissionableDataProvider failed");
	return;
    }
    SetCommissionableDataProvider(&gExampleCommissionableDataProvider);
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
#endif // CONFIG_ENABLE_ATBM_FACTORY_DATA_PROVIDER

    chip::DeviceLayer::PlatformMgr().ScheduleWork(InitServer, reinterpret_cast<intptr_t>(nullptr));

    error = GetAppTask().StartAppTask();
    if (error != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "GetAppTask().StartAppTask() failed : %s", ErrorStr(error));
    }
    else
    {
        ChipLogProgress(DeviceLayer, "StartAppTask over.");
    }
}
