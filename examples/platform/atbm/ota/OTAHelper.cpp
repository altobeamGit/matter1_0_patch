/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

#include "OTAHelper.h"

#include <app/clusters/ota-requestor/BDXDownloader.h>
#include <app/clusters/ota-requestor/DefaultOTARequestor.h>
#include <app/clusters/ota-requestor/DefaultOTARequestorStorage.h>
#include <app/clusters/ota-requestor/ExtendedOTARequestorDriver.h>
#include <platform/atbm/OTAImageProcessorImpl.h>
#include <system/SystemEvent.h>

#include <app/clusters/ota-requestor/DefaultOTARequestorUserConsent.h>
#include <lib/shell/Commands.h>
#include <lib/shell/Engine.h>
#include <lib/shell/commands/Help.h>
#include <lib/support/logging/CHIPLogging.h>

using namespace chip::DeviceLayer;
using namespace chip;

class CustomOTARequestorDriver : public DeviceLayer::ExtendedOTARequestorDriver
{
public:
    bool CanConsent() override;
};

namespace {
DefaultOTARequestor gRequestorCore;
DefaultOTARequestorStorage gRequestorStorage;
CustomOTARequestorDriver gRequestorUser;
BDXDownloader gDownloader;
OTAImageProcessorImpl gImageProcessor;
chip::Optional<bool> gRequestorCanConsent;
static chip::ota::UserConsentState gUserConsentState = chip::ota::UserConsentState::kUnknown;
chip::ota::DefaultOTARequestorUserConsent gUserConsentProvider;

} // namespace

bool CustomOTARequestorDriver::CanConsent()
{
    return gRequestorCanConsent.ValueOr(DeviceLayer::ExtendedOTARequestorDriver::CanConsent());
}

static void InitOTARequestorHandler(System::Layer * systemLayer, void * appState)
{
    SetRequestorInstance(&gRequestorCore);
    gRequestorStorage.Init(Server::GetInstance().GetPersistentStorage());
    gRequestorCore.Init(Server::GetInstance(), gRequestorStorage, gRequestorUser, gDownloader);
    gImageProcessor.SetOTADownloader(&gDownloader);
    gDownloader.SetImageProcessorDelegate(&gImageProcessor);
    gRequestorUser.Init(&gRequestorCore, &gImageProcessor);

    if (gUserConsentState != chip::ota::UserConsentState::kUnknown)
    {
        gUserConsentProvider.SetUserConsentState(gUserConsentState);
        gRequestorUser.SetUserConsentDelegate(&gUserConsentProvider);
    }
}

void OTAHelpers::InitOTARequestor()
{
    chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds32(kInitOTARequestorDelaySec), InitOTARequestorHandler,
                                                nullptr);
}

