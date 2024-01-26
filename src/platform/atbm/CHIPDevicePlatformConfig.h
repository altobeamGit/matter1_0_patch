/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Nest Labs, Inc.
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
 *          Platform-specific configuration overrides for the Chip Device Layer
 *          on ATBM platform.
 */

#pragma once

#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_STATION 1
#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP 0

#ifndef CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE
#define CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE (8 * 1024)
#endif // CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE

#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_TELEMETRY 0

#define CHIP_DEVICE_CONFIG_MAX_EVENT_QUEUE_SIZE 25

#define CONFIG_DEFAULT_WIFI_SSID     "WIFI-TEST-AP122"
#define CONFIG_DEFAULT_WIFI_PASSWORD "1234567890"

#ifndef CHIP_DEVICE_CONFIG_USE_TEST_SETUP_PIN_CODE
#define CHIP_DEVICE_CONFIG_USE_TEST_SETUP_PIN_CODE 20202021
#endif
#ifndef CHIP_DEVICE_CONFIG_USE_TEST_SETUP_DISCRIMINATOR
#define CHIP_DEVICE_CONFIG_USE_TEST_SETUP_DISCRIMINATOR 0xF00
#endif

#define CHIP_DEVICE_CONFIG_ENABLE_EXTENDED_DISCOVERY 1
#define CHIP_DEVICE_CONFIG_ENABLE_COMMISSIONER_DISCOVERY 0
#define CHIP_DEVICE_CONFIG_ENABLE_COMMISSIONER_DISCOVERY_CLIENT 1
#define CHIP_DEVICE_CONFIG_ENABLE_COMMISSIONABLE_DISCOVERY 1
#define CHIP_DEVICE_CONFIG_ENABLE_COMMISSIONABLE_DEVICE_TYPE 1
#define CHIP_DEVICE_CONFIG_DEVICE_TYPE 257 //0x0101 = 257 = Dimmable Buld
#define CHIP_DEVICE_CONFIG_ENABLE_COMMISSIONABLE_DEVICE_NAME 1
#define CHIP_DEVICE_CONFIG_DEVICE_NAME "Test atbm"

