/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *    Copyright (c) 2019-2020 Google LLC.
 *    Copyright (c) 2019 Nest Labs, Inc.
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
 *          Utilities for interacting with the the ATBM Easyflash module.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/atbm/ATBMConfig.h>

#include <lib/core/CHIPEncoding.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/atbm/ATBMUtils.h>

#include <easyflash.h>

// 3R: easyflash errno mapping to the CHIP errno

namespace chip {
namespace DeviceLayer {
namespace Internal {

// *** CAUTION ***: Changing the names or namespaces of these values will *break* existing devices.

// NVS namespaces used to store device configuration information.
const char ATBMConfig::kConfigNamespace_ChipFactory[]  = "chip-factory";
const char ATBMConfig::kConfigNamespace_ChipConfig[]   = "chip-config";
const char ATBMConfig::kConfigNamespace_ChipCounters[] = "chip-counters";

// Keys stored in the chip-factory namespace
const ATBMConfig::Key ATBMConfig::kConfigKey_SerialNum             = { "serial-num" };
const ATBMConfig::Key ATBMConfig::kConfigKey_MfrDeviceId           = { "device-id" };
const ATBMConfig::Key ATBMConfig::kConfigKey_MfrDeviceCert         = { "device-cert" };
const ATBMConfig::Key ATBMConfig::kConfigKey_MfrDeviceICACerts     = { "device-ca-certs" };
const ATBMConfig::Key ATBMConfig::kConfigKey_MfrDevicePrivateKey   = { "device-key" };
const ATBMConfig::Key ATBMConfig::kConfigKey_HardwareVersion       = { "hardware-ver" };
const ATBMConfig::Key ATBMConfig::kConfigKey_HardwareVersionString = { "hw-ver-str" };
const ATBMConfig::Key ATBMConfig::kConfigKey_ManufacturingDate     = { "mfg-date" };
const ATBMConfig::Key ATBMConfig::kConfigKey_SetupPinCode          = { "pin-code" };
const ATBMConfig::Key ATBMConfig::kConfigKey_SetupDiscriminator    = { "discriminator" };
const ATBMConfig::Key ATBMConfig::kConfigKey_Spake2pIterationCount = { "iteration-count" };
const ATBMConfig::Key ATBMConfig::kConfigKey_Spake2pSalt           = { "salt" };
const ATBMConfig::Key ATBMConfig::kConfigKey_Spake2pVerifier       = { "verifier" };
const ATBMConfig::Key ATBMConfig::kConfigKey_DACCert               = { "dac-cert" };
const ATBMConfig::Key ATBMConfig::kConfigKey_DACPrivateKey         = { "dac-key" };
const ATBMConfig::Key ATBMConfig::kConfigKey_DACPublicKey          = { "dac-pub-key" };
const ATBMConfig::Key ATBMConfig::kConfigKey_PAICert               = { "pai-cert" };
const ATBMConfig::Key ATBMConfig::kConfigKey_CertDeclaration       = { "cert-dclrn" };
const ATBMConfig::Key ATBMConfig::kConfigKey_VendorId              = { "vendor-id" };
const ATBMConfig::Key ATBMConfig::kConfigKey_VendorName            = { "vendor-name" };
const ATBMConfig::Key ATBMConfig::kConfigKey_ProductId             = { "product-id" };
const ATBMConfig::Key ATBMConfig::kConfigKey_ProductName           = { "product-name" };
const ATBMConfig::Key ATBMConfig::kConfigKey_ProductLabel          = { "product-label" };
const ATBMConfig::Key ATBMConfig::kConfigKey_ProductURL            = { "product-url" };
const ATBMConfig::Key ATBMConfig::kConfigKey_SupportedCalTypes     = { "cal-types" };
const ATBMConfig::Key ATBMConfig::kConfigKey_SupportedLocaleSize   = { "locale-sz" };
const ATBMConfig::Key ATBMConfig::kConfigKey_RotatingDevIdUniqueId = { "rd-id-uid" };
const ATBMConfig::Key ATBMConfig::kConfigKey_LocationCapability    = { "loc-capability" };

// Keys stored in the chip-config namespace
const ATBMConfig::Key ATBMConfig::kConfigKey_ServiceConfig               = { "service-config" };
const ATBMConfig::Key ATBMConfig::kConfigKey_PairedAccountId             = { "account-id" };
const ATBMConfig::Key ATBMConfig::kConfigKey_ServiceId                   = { "service-id" };
const ATBMConfig::Key ATBMConfig::kConfigKey_LastUsedEpochKeyId          = { "last-ek-id" };
const ATBMConfig::Key ATBMConfig::kConfigKey_FailSafeArmed               = { "fail-safe-armed" };
const ATBMConfig::Key ATBMConfig::kConfigKey_WiFiStationSecType          = { "sta-sec-type" };
const ATBMConfig::Key ATBMConfig::kConfigKey_RegulatoryLocation          = { "reg-location" };
const ATBMConfig::Key ATBMConfig::kConfigKey_CountryCode                 = { "country-code" };
const ATBMConfig::Key ATBMConfig::kConfigKey_UniqueId                    = { "unique-id" };

// Keys stored in the Chip-counters namespace
const ATBMConfig::Key ATBMConfig::kCounterKey_RebootCount           = { "reboot-count" };
const ATBMConfig::Key ATBMConfig::kCounterKey_UpTime                = { "up-time" };
const ATBMConfig::Key ATBMConfig::kCounterKey_TotalOperationalHours = { "total-hours" };

CHIP_ERROR ATBMConfig::ReadConfigValue(Key key, bool & val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    bool tmpVal;
    size_t ret, valLen;

    ret = ef_get_env_blob(key.name, &tmpVal, sizeof(tmpVal), &valLen);
    if (ret <= 0)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    val = tmpVal;

exit:
    return err;
}

CHIP_ERROR ATBMConfig::ReadConfigValue(Key key, uint32_t & val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    uint32_t tmpVal;
    size_t ret, valLen;

    ret = ef_get_env_blob(key.name, &tmpVal, sizeof(tmpVal), &valLen);
    if (ret <= 0)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    val = tmpVal;

exit:
    return err;
}

CHIP_ERROR ATBMConfig::ReadConfigValue(Key key, uint64_t & val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    uint64_t tmpVal;
    size_t ret, valLen;

    ret = ef_get_env_blob(key.name, &tmpVal, sizeof(tmpVal), &valLen);
    if (ret <= 0)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    val = tmpVal;
exit:
    return err;
}

CHIP_ERROR ATBMConfig::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    CHIP_ERROR err       = CHIP_NO_ERROR;
    char *tmpVal = NULL;
    size_t ret;
    
    tmpVal = (char *)malloc(bufSize);
    if (tmpVal == NULL)
    {
    	ChipLogError(DeviceLayer, "ReadConfigValueStr malloc fail");
    	err = CHIP_ERROR_NO_MEMORY;
    	goto exit;
    }

    ret = ef_get_env_blob(key.name, tmpVal, bufSize - 1, NULL);
    if ('\0' == tmpVal[0])
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    outLen = ret;
    Platform::CopyString(buf, outLen, tmpVal);
    free(tmpVal);

exit:
    return err;
}

CHIP_ERROR ATBMConfig::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    CHIP_ERROR err       = CHIP_NO_ERROR;
    char *tmpVal = NULL;
    size_t ret;
    size_t savedLen = 0;
    
    tmpVal = (char *)malloc(bufSize);
    if (tmpVal == NULL)
    {
    	ChipLogError(DeviceLayer, "ReadConfigValueBin malloc fail");
    	err = CHIP_ERROR_NO_MEMORY;
    	goto exit;
    }

    ret = ef_get_env_blob(key.name, tmpVal, bufSize, &savedLen);
    if (0 == savedLen)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    outLen = ret;
    memcpy(buf, tmpVal, outLen);
    free(tmpVal);

exit:
    return err;
}

CHIP_ERROR ATBMConfig::WriteConfigValue(Key key, bool val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    EfErrCode ret = ef_set_env_blob(key.name, &val, sizeof(val));
    if (ret != EF_NO_ERR)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    // ChipLogProgress(DeviceLayer, "Easyflash set: %s = %s", key.name, val ? "true" : "false");
exit:
    return err;
}

CHIP_ERROR ATBMConfig::WriteConfigValue(Key key, uint32_t val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    EfErrCode ret = ef_set_env_blob(key.name, &val, sizeof(val));
    if (ret != EF_NO_ERR)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    // ChipLogProgress(DeviceLayer, "Easyflash set: %s = %" PRIu32 " (0x%" PRIX32 ")", key.name, val, val);

exit:
    return err;
}

CHIP_ERROR ATBMConfig::WriteConfigValue(Key key, uint64_t val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    EfErrCode ret = ef_set_env_blob(key.name, &val, sizeof(val));
    if (ret != EF_NO_ERR)
    {
        ChipLogError(DeviceLayer, "WriteConfigValue() failed. key: %s", key.name);
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }
    SuccessOrExit(err);

    // ChipLogProgress(DeviceLayer, "Easyflash set: %s = %" PRIu64 " (0x%" PRIX64 ")", key.name, val, val);

exit:
    return err;
}

CHIP_ERROR ATBMConfig::WriteConfigValueStr(Key key, const char * str)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    if (str != NULL)
    {
        EfErrCode ret = ef_set_env(key.name, str);
        if (ret != EF_NO_ERR)
        {
            err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
        }
        SuccessOrExit(err);

        // ChipLogProgress(DeviceLayer, "Easyflash set: %s = \"%s\"", key.name, str);
    }
    else
    {
        err = ClearConfigValue(key);
        SuccessOrExit(err);
    }

exit:
    return err;
}

CHIP_ERROR ATBMConfig::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    CHIP_ERROR err;
    chip::Platform::ScopedMemoryBuffer<char> strCopy;

    if (str != NULL)
    {
        strCopy.Calloc(strLen + 1);
        VerifyOrExit(strCopy, err = CHIP_ERROR_NO_MEMORY);
        Platform::CopyString(strCopy.Get(), strLen + 1, str);
    }
    err = ATBMConfig::WriteConfigValueStr(key, strCopy.Get());

exit:
    return err;
}

CHIP_ERROR ATBMConfig::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    if (data != NULL)
    {
        EfErrCode ret = ef_set_env_blob(key.name, data, dataLen);
        if (ret != EF_NO_ERR)
        {
            err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
        }
        SuccessOrExit(err);

        // ChipLogProgress(DeviceLayer, "Easyflash set: /%s = (blob length %" PRId32 ")", key.name, (unsigned long )dataLen);
    }
    else
    {
        err = ClearConfigValue(key);
        SuccessOrExit(err);
    }

exit:
    return err;
}

CHIP_ERROR ATBMConfig::ClearConfigValue(Key key)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    EfErrCode ret  = ef_del_env(key.name);
    if (ret != EF_NO_ERR)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }

    SuccessOrExit(err);

    ChipLogProgress(DeviceLayer, "Easyflash erase: %s", key.name);

exit:
    return err;
}

bool ATBMConfig::ConfigValueExists(Key key)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    env_node_obj node;
    bool result = ef_get_env_obj(key.name, &node);
    if (!result)
    {
        err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }

    return err == CHIP_NO_ERROR;
}

CHIP_ERROR ATBMConfig::EnsureNamespace(const char * ns)
{
    return CHIP_NO_ERROR;
}

CHIP_ERROR ATBMConfig::ClearNamespace(const char * ns)
{
    return CHIP_NO_ERROR;
}

void ATBMConfig::RunConfigUnitTest() {}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
