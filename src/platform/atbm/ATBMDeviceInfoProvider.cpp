/*

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
#include <lib/core/CHIPTLV.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/DefaultStorageKeyAllocator.h>
#include <platform/atbm/ATBMConfig.h>
#include <platform/atbm/ATBMDeviceInfoProvider.h>
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <string.h>

namespace chip {
namespace DeviceLayer {

namespace {
constexpr TLV::Tag kLabelNameTag  = TLV::ContextTag(0);
constexpr TLV::Tag kLabelValueTag = TLV::ContextTag(1);
} // anonymous namespace

using namespace Internal;

ATBMDeviceInfoProvider & ATBMDeviceInfoProvider::GetDefaultInstance(void)
{
    static ATBMDeviceInfoProvider sInstance;
    return sInstance;
}

DeviceInfoProvider::FixedLabelIterator * ATBMDeviceInfoProvider::IterateFixedLabel(EndpointId endpoint)
{
    return chip::Platform::New<FixedLabelIteratorImpl>(endpoint);
}

ATBMDeviceInfoProvider::FixedLabelIteratorImpl::FixedLabelIteratorImpl(EndpointId endpoint) : mEndpoint(endpoint)
{
    mIndex = 0;
}

size_t ATBMDeviceInfoProvider::FixedLabelIteratorImpl::Count()
{
    char keyBuf[ATBMConfig::kMaxConfigKeyNameLength];
    uint32_t count = 0;

    VerifyOrReturnValue(ATBMConfig::KeyAllocator::FixedLabelCount(keyBuf, sizeof(keyBuf), mEndpoint) == CHIP_NO_ERROR, 0);
    ATBMConfig::Key key( keyBuf );
    VerifyOrReturnValue(ATBMConfig::ReadConfigValue(key, count) == CHIP_NO_ERROR, 0);
    return count;
}

bool ATBMDeviceInfoProvider::FixedLabelIteratorImpl::Next(FixedLabelType & output)
{
    ChipLogDetail(DeviceLayer, "Get the fixed label with index:%u at endpoint:%d", static_cast<unsigned>(mIndex), mEndpoint);

    char keyBuf[ATBMConfig::kMaxConfigKeyNameLength];
    size_t keyOutLen   = 0;
    size_t valueOutLen = 0;

    memset(mFixedLabelNameBuf, 0, sizeof(mFixedLabelNameBuf));
    memset(mFixedLabelValueBuf, 0, sizeof(mFixedLabelValueBuf));

    VerifyOrReturnValue(ATBMConfig::KeyAllocator::FixedLabelKey(keyBuf, sizeof(keyBuf), mEndpoint, mIndex) == CHIP_NO_ERROR,
                        false);
    ATBMConfig::Key keyKey( keyBuf );
    VerifyOrReturnValue(
        ATBMConfig::ReadConfigValueStr(keyKey, mFixedLabelNameBuf, sizeof(mFixedLabelNameBuf), keyOutLen) == CHIP_NO_ERROR, false);

    VerifyOrReturnValue(ATBMConfig::KeyAllocator::FixedLabelValue(keyBuf, sizeof(keyBuf), mEndpoint, mIndex) == CHIP_NO_ERROR,
                        false);
    ATBMConfig::Key valueKey( keyBuf );
    VerifyOrReturnValue(ATBMConfig::ReadConfigValueStr(valueKey, mFixedLabelValueBuf, sizeof(mFixedLabelValueBuf), valueOutLen) ==
                            CHIP_NO_ERROR,
                        false);

    output.label = CharSpan::fromCharString(mFixedLabelNameBuf);
    output.value = CharSpan::fromCharString(mFixedLabelValueBuf);
    ChipLogDetail(DeviceLayer, "Fixed label with index:%u at endpoint:%d, %s:%s", static_cast<unsigned>(mIndex), mEndpoint,
                  mFixedLabelNameBuf, mFixedLabelValueBuf);

    mIndex++;
    return true;
}

CHIP_ERROR ATBMDeviceInfoProvider::SetUserLabelLength(EndpointId endpoint, size_t val)
{
    VerifyOrReturnError(mStorage != nullptr, CHIP_ERROR_INCORRECT_STATE);
    DefaultStorageKeyAllocator keyAlloc;
    return mStorage->SyncSetKeyValue(keyAlloc.UserLabelLengthKey(endpoint), &val, static_cast<uint16_t>(sizeof(val)));
}

CHIP_ERROR ATBMDeviceInfoProvider::GetUserLabelLength(EndpointId endpoint, size_t & val)
{
    VerifyOrReturnError(mStorage != nullptr, CHIP_ERROR_INCORRECT_STATE);
    DefaultStorageKeyAllocator keyAlloc;
    uint16_t len = static_cast<uint16_t>(sizeof(val));
    return mStorage->SyncGetKeyValue(keyAlloc.UserLabelLengthKey(endpoint), &val, len);
}

CHIP_ERROR ATBMDeviceInfoProvider::SetUserLabelAt(EndpointId endpoint, size_t index, const UserLabelType & userLabel)
{
    VerifyOrReturnError(mStorage != nullptr, CHIP_ERROR_INCORRECT_STATE);
    DefaultStorageKeyAllocator keyAlloc;
    uint8_t buf[UserLabelTLVMaxSize()];
    TLV::TLVWriter writer;
    writer.Init(buf);

    TLV::TLVType outerType;
    ReturnErrorOnFailure(writer.StartContainer(TLV::AnonymousTag(), TLV::kTLVType_Structure, outerType));
    ReturnErrorOnFailure(writer.PutString(kLabelNameTag, userLabel.label));
    ReturnErrorOnFailure(writer.PutString(kLabelValueTag, userLabel.value));
    ReturnErrorOnFailure(writer.EndContainer(outerType));

    return mStorage->SyncSetKeyValue(keyAlloc.UserLabelIndexKey(endpoint, index), buf,
                                     static_cast<uint16_t>(writer.GetLengthWritten()));
}

CHIP_ERROR ATBMDeviceInfoProvider::DeleteUserLabelAt(EndpointId endpoint, size_t index)
{
    VerifyOrReturnError(mStorage != nullptr, CHIP_ERROR_INCORRECT_STATE);
    DefaultStorageKeyAllocator keyAlloc;
    return mStorage->SyncDeleteKeyValue(keyAlloc.UserLabelIndexKey(endpoint, index));
}

DeviceInfoProvider::UserLabelIterator * ATBMDeviceInfoProvider::IterateUserLabel(EndpointId endpoint)
{
    return chip::Platform::New<UserLabelIteratorImpl>(*this, endpoint);
}

ATBMDeviceInfoProvider::UserLabelIteratorImpl::UserLabelIteratorImpl(ATBMDeviceInfoProvider & provider, EndpointId endpoint) :
    mProvider(provider), mEndpoint(endpoint)
{
    size_t total = 0;

    ReturnOnFailure(mProvider.GetUserLabelLength(mEndpoint, total));
    mTotal = total;
    mIndex = 0;
}

bool ATBMDeviceInfoProvider::UserLabelIteratorImpl::Next(UserLabelType & output)
{
    VerifyOrReturnError(mProvider.mStorage != nullptr, false);
    VerifyOrReturnError(mIndex < mTotal, false);

    DefaultStorageKeyAllocator keyAlloc;
    uint8_t buf[UserLabelTLVMaxSize()];
    uint16_t len = static_cast<uint16_t>(sizeof(buf));

    CHIP_ERROR err = mProvider.mStorage->SyncGetKeyValue(keyAlloc.UserLabelIndexKey(mEndpoint, mIndex), buf, len);
    VerifyOrReturnError(err == CHIP_NO_ERROR, false);

    TLV::ContiguousBufferTLVReader reader;
    reader.Init(buf);
    err = reader.Next(TLV::kTLVType_Structure, TLV::AnonymousTag());
    VerifyOrReturnError(err == CHIP_NO_ERROR, false);

    TLV::TLVType containerType;
    VerifyOrReturnError(reader.EnterContainer(containerType) == CHIP_NO_ERROR, false);

    chip::CharSpan label;
    chip::CharSpan value;

    VerifyOrReturnError(reader.Next(kLabelNameTag) == CHIP_NO_ERROR, false);
    VerifyOrReturnError(reader.Get(label) == CHIP_NO_ERROR, false);

    VerifyOrReturnError(reader.Next(kLabelValueTag) == CHIP_NO_ERROR, false);
    VerifyOrReturnError(reader.Get(value) == CHIP_NO_ERROR, false);

    VerifyOrReturnError(reader.VerifyEndOfContainer() == CHIP_NO_ERROR, false);
    VerifyOrReturnError(reader.ExitContainer(containerType) == CHIP_NO_ERROR, false);

    Platform::CopyString(mUserLabelNameBuf, label);
    Platform::CopyString(mUserLabelValueBuf, value);

    output.label = CharSpan::fromCharString(mUserLabelNameBuf);
    output.value = CharSpan::fromCharString(mUserLabelValueBuf);

    mIndex++;

    return true;
}

DeviceInfoProvider::SupportedLocalesIterator * ATBMDeviceInfoProvider::IterateSupportedLocales()
{
    return chip::Platform::New<SupportedLocalesIteratorImpl>();
}

size_t ATBMDeviceInfoProvider::SupportedLocalesIteratorImpl::Count()
{
    uint32_t count = 0;
    CHIP_ERROR err = ATBMConfig::ReadConfigValue(ATBMConfig::kConfigKey_SupportedLocaleSize, count);
    if (err != CHIP_NO_ERROR)
    {
        return 0;
    }
    return count;
}

bool ATBMDeviceInfoProvider::SupportedLocalesIteratorImpl::Next(CharSpan & output)
{
    char keyBuf[ATBMConfig::kMaxConfigKeyNameLength];
    size_t keyOutLen = 0;
    memset(mLocaleBuf, 0, sizeof(mLocaleBuf));

    VerifyOrReturnValue(ATBMConfig::KeyAllocator::Locale(keyBuf, sizeof(keyBuf), mIndex) == CHIP_NO_ERROR, false);
    ATBMConfig::Key keyKey( keyBuf );
    VerifyOrReturnValue(ATBMConfig::ReadConfigValueStr(keyKey, mLocaleBuf, sizeof(mLocaleBuf), keyOutLen) == CHIP_NO_ERROR, false);

    output = CharSpan::fromCharString(mLocaleBuf);
    mIndex++;
    return true;
}

void ATBMDeviceInfoProvider::SupportedLocalesIteratorImpl::Release()
{
    chip::Platform::Delete(this);
}

DeviceInfoProvider::SupportedCalendarTypesIterator * ATBMDeviceInfoProvider::IterateSupportedCalendarTypes()
{
    return chip::Platform::New<SupportedCalendarTypesIteratorImpl>();
}

ATBMDeviceInfoProvider::SupportedCalendarTypesIteratorImpl::SupportedCalendarTypesIteratorImpl()
{
    CHIP_ERROR err = ATBMConfig::ReadConfigValue(ATBMConfig::kConfigKey_SupportedCalTypes, mSupportedCalendarTypes);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to read supported calendar types: %" CHIP_ERROR_FORMAT, err.Format());
    }
}

size_t ATBMDeviceInfoProvider::SupportedCalendarTypesIteratorImpl::Count()
{
    size_t count = 0;
    for (uint8_t i = 0; i < to_underlying(CalendarType::kUnknownEnumValue); i++)
    {
        if (mSupportedCalendarTypes & (1 << i))
        {
            count++;
        }
    }
    ChipLogDetail(DeviceLayer, "Supported calendar types count:%u", count);
    return count;
}

bool ATBMDeviceInfoProvider::SupportedCalendarTypesIteratorImpl::Next(CalendarType & output)
{
    while (mIndex < to_underlying(CalendarType::kUnknownEnumValue))
    {
        if (mSupportedCalendarTypes & (1 << mIndex))
        {
            output = static_cast<CalendarType>(mIndex);
            mIndex++;
            return true;
        }
        mIndex++;
    }
    return false;
}

} // namespace DeviceLayer
} // namespace chip
