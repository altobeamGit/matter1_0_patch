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
 *          Provides the implementation of the Device Layer ConfigurationManager object
 *          for the ATBM platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <lib/core/CHIPKeyIds.h>
#include <lib/support/CodeUtils.h>
#include <platform/ConfigurationManager.h>
#include <platform/atbm/ATBMConfig.h>
#include <platform/internal/GenericConfigurationManagerImpl.ipp>
#include "atbm_general.h"

extern "C" {
#include <easyflash.h>
}

namespace chip {
namespace DeviceLayer {

using namespace ::chip::DeviceLayer::Internal;

namespace {

enum
{
    kChipProduct_Connect = 0x0016
};

} // unnamed namespace

// TODO: Define a Singleton instance of CHIP Group Key Store here (#1266)

ConfigurationManagerImpl & ConfigurationManagerImpl::GetDefaultInstance()
{
    static ConfigurationManagerImpl sInstance;
    return sInstance;
}

CHIP_ERROR ConfigurationManagerImpl::Init()
{
    CHIP_ERROR err;
    uint32_t rebootCount;

    // Force initialization of chip namespaces if they doesn't already exist.
    err = ATBMConfig::EnsureNamespace(ATBMConfig::kConfigNamespace_ChipFactory);
    SuccessOrExit(err);
    err = ATBMConfig::EnsureNamespace(ATBMConfig::kConfigNamespace_ChipConfig);
    SuccessOrExit(err);
    err = ATBMConfig::EnsureNamespace(ATBMConfig::kConfigNamespace_ChipCounters);
    SuccessOrExit(err);

    if (ATBMConfig::ConfigValueExists(ATBMConfig::kCounterKey_RebootCount))
    {
        err = GetRebootCount(rebootCount);
        SuccessOrExit(err);

        err = StoreRebootCount(rebootCount + 1);
        SuccessOrExit(err);
    }
    else
    {
        // The first boot after factory reset of the Node.
        err = StoreRebootCount(1);
        SuccessOrExit(err);
    }

    if (!ATBMConfig::ConfigValueExists(ATBMConfig::kCounterKey_TotalOperationalHours))
    {
        err = StoreTotalOperationalHours(0);
        SuccessOrExit(err);
    }

    // Initialize the generic implementation base class.
    err = Internal::GenericConfigurationManagerImpl<ATBMConfig>::Init();
    SuccessOrExit(err);

    err = CHIP_NO_ERROR;

exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetRebootCount(uint32_t & rebootCount)
{
    return ATBMConfig::ReadConfigValue(ATBMConfig::kCounterKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::StoreRebootCount(uint32_t rebootCount)
{
    return ATBMConfig::WriteConfigValue(ATBMConfig::kCounterKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::GetTotalOperationalHours(uint32_t & totalOperationalHours)
{
    return ReadConfigValue(ATBMConfig::kCounterKey_TotalOperationalHours, totalOperationalHours);
}

CHIP_ERROR ConfigurationManagerImpl::StoreTotalOperationalHours(uint32_t totalOperationalHours)
{
    return WriteConfigValue(ATBMConfig::kCounterKey_TotalOperationalHours, totalOperationalHours);
}

CHIP_ERROR ConfigurationManagerImpl::GetProductURL(char * buf, size_t bufSize)
{
    CHIP_ERROR err = ReadConfigValueStr(ATBMConfig::kConfigKey_ProductURL, buf, bufSize, bufSize);
    if (err == CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND)
    {
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetProductLabel(char * buf, size_t bufSize)
{
    CHIP_ERROR err = ReadConfigValueStr(ATBMConfig::kConfigKey_ProductLabel, buf, bufSize, bufSize);
    if (err == CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND)
    {
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetSoftwareVersionString(char * buf, size_t bufSize)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
}

CHIP_ERROR ConfigurationManagerImpl::GetSoftwareVersion(uint32_t & softwareVer)
{
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;

}

CHIP_ERROR ConfigurationManagerImpl::GetLocationCapability(uint8_t & location)
{
#if CONFIG_ENABLE_ATBM_LOCATIONCAPABILITY
    uint32_t value = 0;
    CHIP_ERROR err = ReadConfigValue(ATBMConfig::kConfigKey_LocationCapability, value);
    if (err == CHIP_NO_ERROR)
    {
    	VerifyOrReturnError(value <= UINT8_MAX, CHIP_ERROR_INVALID_INTEGER_VALUE);
	location = static_cast<uint8_t>(value);
    }
    return err;
#else
    location = static_cast<uint8_t>(chip::app::Clusters::GeneralCommissioning::RegulatoryLocationType::kIndoorOutdoor);
    return CHIP_NO_ERROR;
#endif
}

CHIP_ERROR ConfigurationManagerImpl::GetPrimaryWiFiMACAddress(uint8_t * buf)
{
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI
    atbm_wifi_get_mac_addr(buf);
    return CHIP_NO_ERROR;
#else
    return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
#endif
}

bool ConfigurationManagerImpl::CanFactoryReset()
{
    // TODO: query the application to determine if factory reset is allowed.
    return true;
}

void ConfigurationManagerImpl::InitiateFactoryReset()
{
    PlatformMgr().ScheduleWork(DoFactoryReset);
}

CHIP_ERROR ConfigurationManagerImpl::ReadPersistedStorageValue(::chip::Platform::PersistedStorage::Key key, uint32_t & value)
{
    ATBMConfig::Key configKey{ key };

    CHIP_ERROR err = ReadConfigValue(configKey, value);
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::WritePersistedStorageValue(::chip::Platform::PersistedStorage::Key key, uint32_t value)
{
    ATBMConfig::Key configKey{ key };
    return WriteConfigValue(configKey, value);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, bool & val)
{
    return ATBMConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint32_t & val)
{
    return ATBMConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint64_t & val)
{
    return ATBMConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return ATBMConfig::ReadConfigValueStr(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return ATBMConfig::ReadConfigValueBin(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, bool val)
{
    return ATBMConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint32_t val)
{
    return ATBMConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint64_t val)
{
    return ATBMConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str)
{
    return ATBMConfig::WriteConfigValueStr(key, str);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return ATBMConfig::WriteConfigValueStr(key, str, strLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return ATBMConfig::WriteConfigValueBin(key, data, dataLen);
}

void ConfigurationManagerImpl::RunConfigUnitTest(void)
{
    ATBMConfig::RunConfigUnitTest();
}

void ConfigurationManagerImpl::DoFactoryReset(intptr_t arg)
{
    CHIP_ERROR err;

    ChipLogProgress(DeviceLayer, "Performing factory reset");
    ef_env_set_default();
    ChipLogProgress(DeviceLayer, "System restarting");
    hal_sys_reboot();
}

ConfigurationManager & ConfigurationMgrImpl()
{
    return ConfigurationManagerImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip
