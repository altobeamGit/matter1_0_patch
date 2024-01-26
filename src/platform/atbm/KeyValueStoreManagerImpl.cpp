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
 *    @file
 *          Platform-specific key value storage implementation for ATBM
 */

#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <platform/KeyValueStoreManager.h>

#include <string.h>

#include <platform/atbm/ATBMConfig.h>
//#include <platform/atbm/KeyValueStoreManagerImpl.h>

/* ignore GCC Wconversion warnings for pigweed */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif


namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {

using namespace ::chip::DeviceLayer::Internal;

KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char * key, void * value, size_t value_size, size_t * read_bytes_size, size_t offset_bytes) const
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // todo: need get value at offset for return
    size_t outlen         = 0;
    ATBMConfig::Key ckey = { key };

    err = ATBMConfig::ReadConfigValueBin(ckey, (uint8_t *)value, value_size, outlen);
    if (CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND == err)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }

    SuccessOrExit(err);

    if (read_bytes_size)
    {
        *read_bytes_size = outlen;
    }

exit:
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char * key, const void * value, size_t value_size)
{
    CHIP_ERROR err        = CHIP_NO_ERROR;
    ATBMConfig::Key ckey = { key };

    err = ATBMConfig::WriteConfigValueBin(ckey, (const uint8_t *)value, value_size);
    if (CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND == err)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }

    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char * key)
{
    CHIP_ERROR err        = CHIP_NO_ERROR;
    ATBMConfig::Key ckey = { key };

    err = ATBMConfig::ClearConfigValue(ckey);

    if (CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND == err)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }

    return err;
}

} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip
// namespace chip
