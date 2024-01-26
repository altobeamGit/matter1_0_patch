/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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
 * @file DeviceCallbacks.cpp
 *
 * Implements all the callbacks to the application from the CHIP Stack
 *
 **/

#include "AppTask.h"

#include "DeviceCallbacks.h"
#include "LEDWidget.h"

#include <app/util/util.h>

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <lib/support/logging/CHIPLogging.h>

#include "atbm_general.h"

extern LEDWidget AppLED;

using namespace chip;
using namespace chip::Inet;
using namespace chip::System;
using namespace chip::app::Clusters;

void AppDeviceCallbacks::PostAttributeChangeCallback(EndpointId endpointId, ClusterId clusterId, AttributeId attributeId,
                                                     uint8_t type, uint16_t size, uint8_t * value)
{
    ChipLogProgress(DeviceLayer, "PostAttributeChangeCallback - Cluster ID: '0x%04x', EndPoint ID: '0x%02x', Attribute ID: '0x%04x'\n", clusterId,
             endpointId, attributeId);

    switch (clusterId)
    {
    case OnOff::Id:
        ChipLogProgress(DeviceLayer, "PostAttributeChangeCallback OnOff---");
        OnOnOffPostAttributeChangeCallback(endpointId, attributeId, value);
        break;

    case LevelControl::Id:
        ChipLogProgress(DeviceLayer, "PostAttributeChangeCallback LevelControl---");
        OnLevelControlAttributeChangeCallback(endpointId, attributeId, value);
        break;

    default:
        ChipLogProgress(DeviceLayer, "Unhandled cluster ID: %d\n", (int)clusterId);
        break;
    }
}

void AppDeviceCallbacks::OnOnOffPostAttributeChangeCallback(EndpointId endpointId, AttributeId attributeId, uint8_t * value)
{
    VerifyOrExit(attributeId == OnOff::Attributes::OnOff::Id, ChipLogError(DeviceLayer, "Unhandled Attribute ID: '0x%04x'", attributeId));
    VerifyOrExit(endpointId == 1, ChipLogError(DeviceLayer, "Unexpected EndPoint ID: '0x%02x'", endpointId));

    AppLED.Set(*value);

exit:
    return;
}

void AppDeviceCallbacks::OnLevelControlAttributeChangeCallback(EndpointId endpointId, AttributeId attributeId, uint8_t * value)
{
    VerifyOrExit(attributeId == LevelControl::Attributes::CurrentLevel::Id,
                 ChipLogError(DeviceLayer, "Unhandled Attribute ID: '0x%04x'", attributeId));
    VerifyOrExit(endpointId == 1, ChipLogError(DeviceLayer, "Unexpected EndPoint ID: '0x%02x'", endpointId));

    AppLED.SetBrightness(*value);

exit:
    return;
}

/** @brief OnOff Cluster Init
 *
 * This function is called when a specific cluster is initialized. It gives the
 * application an opportunity to take care of cluster initialization procedures.
 * It is called exactly once for each endpoint where cluster is present.
 *
 * @param endpoint   Ver.: always
 *
 * emberAfOnOffClusterInitCallback happens before the stack initialize the cluster
 * attributes to the default value.
 * The logic here expects something similar to the deprecated Plugins callback
 * emberAfPluginOnOffClusterServerPostInitCallback.
 *
 */
void emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
    ChipLogProgress(DeviceLayer, "emberAfOnOffClusterInitCallback");
    GetAppTask().UpdateClusterState();
}
