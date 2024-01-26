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

#include <lib/support/logging/CHIPLogging.h>
#include "atbm_general.h"
#include "LEDWidget.h"

#ifndef UINT8_MAX
#define UINT8_MAX (255)
#endif

void LEDWidget::Init(void)
{
    mState      = false;
    mBrightness = UINT8_MAX;
}

void LEDWidget::Set(bool state)
{
    ChipLogProgress(DeviceLayer, "Setting state to %d\n", state ? 1 : 0);
    if (state == mState)
    {
        return;
    }

    mState = state;

    DoSet();
}

void LEDWidget::Toggle()
{
    ChipLogProgress(DeviceLayer, "Toggling state to %d", !mState);
    mState = !mState;

    DoSet();
}

void LEDWidget::SetBrightness(unsigned char brightness)
{
    ChipLogProgress(DeviceLayer, "Setting brightness to %d\n", (int)brightness);
    if (brightness == mBrightness)
    {
        return;
    }

    mBrightness = brightness;

    DoSet();
}

unsigned char LEDWidget::GetLevel()
{
    return this->mBrightness;
}

bool LEDWidget::IsTurnedOn()
{
    return this->mState;
}

void LEDWidget::DoSet(void)
{
    unsigned char brightness = mState ? mBrightness : 0;
    if (brightness)
    {
        ChipLogProgress(DeviceLayer, "Set LED ON...");
    }
    else
    {
        ChipLogProgress(DeviceLayer, "Set LED OFF...");
    }
}
