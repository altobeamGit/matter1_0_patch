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

#include <app/clusters/ota-requestor/OTADownloader.h>
#include <app/clusters/ota-requestor/OTARequestorInterface.h>
#include <platform/CHIPDeviceEvent.h>

#include "OTAImageProcessorImpl.h"
#include "lib/core/CHIPError.h"

using namespace chip::System;
using namespace ::chip::DeviceLayer::Internal;

namespace chip {

int ota_finalize()
{
    return -1;
}

int ota_write(uint8_t *data, uint32_t size)
{
    return -1;
}

int ota_abort()
{
    return -1;
}

bool OTAImageProcessorImpl::IsFirstImageRun()
{
    OTARequestorInterface * requestor = GetRequestorInstance();
    if (requestor == nullptr)
    {
        return false;
    }

    return requestor->GetCurrentUpdateState() == OTARequestorInterface::OTAUpdateStateEnum::kApplying;
}

CHIP_ERROR OTAImageProcessorImpl::ConfirmCurrentImage()
{
    OTARequestorInterface * requestor = GetRequestorInstance();
    if (requestor == nullptr)
    {
        return CHIP_ERROR_INTERNAL;
    }

    uint32_t currentVersion;
    uint32_t targetVersion = requestor->GetTargetVersion();
    ReturnErrorOnFailure(DeviceLayer::ConfigurationMgr().GetSoftwareVersion(currentVersion));
    if (currentVersion != targetVersion)
    {
        ChipLogError(SoftwareUpdate, "Current software version = %" PRIu32 ", expected software version = %" PRIu32, currentVersion,
                     targetVersion);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::PrepareDownload()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandlePrepareDownload, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Finalize()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleFinalize, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Apply()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleApply, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Abort()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleAbort, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ProcessBlock(ByteSpan & block)
{
    if ((nullptr == block.data()) || block.empty())
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    CHIP_ERROR err = SetBlock(block);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot set block data: %" CHIP_ERROR_FORMAT, err.Format());
        return err;
    }
    DeviceLayer::PlatformMgr().ScheduleWork(HandleProcessBlock, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandlePrepareDownload(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }

    imageProcessor->mParams.downloadedBytes = 0;
    imageProcessor->mParams.totalFileBytes = 0;
    imageProcessor->mHeaderParser.Init();

    imageProcessor->mDownloader->OnPreparedForDownload(CHIP_NO_ERROR);
}

void OTAImageProcessorImpl::HandleFinalize(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }

    //if ota failed
    if (ota_finalize() < 0)
    {
        imageProcessor->mDownloader->EndDownload(CHIP_ERROR_WRITE_FAILED);
        ChipLogProgress(SoftwareUpdate, "OTA image verification error");
    }
    else
    {
        ChipLogProgress(SoftwareUpdate, "OTA image downloaded");
    }

    imageProcessor->ReleaseBlock();
}

void OTAImageProcessorImpl::HandleApply(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);

    if (imageProcessor == nullptr)
    {
        return;
    }

    DeviceLayer::SystemLayer().StartTimer(
        System::Clock::Seconds32(2),
        [](Layer *, void *) {
            ChipLogProgress(SoftwareUpdate, "Rebooting...");
            hal_system_reboot();
        },
        nullptr);
}

void OTAImageProcessorImpl::HandleAbort(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }

    ota_abort();

    imageProcessor->ReleaseBlock();
}

CHIP_ERROR OTAImageProcessorImpl::ProcessHeader(ByteSpan & block)
{
    if (mHeaderParser.IsInitialized())
    {
        OTAImageHeader header;
        CHIP_ERROR error = mHeaderParser.AccumulateAndDecode(block, header);

        // Need more data to decode the header
        ReturnErrorCodeIf(error == CHIP_ERROR_BUFFER_TOO_SMALL, CHIP_NO_ERROR);
        ReturnErrorOnFailure(error);

        mParams.totalFileBytes = header.mPayloadSize;
        mHeaderParser.Clear();
    }

    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandleProcessBlock(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }

    ByteSpan block = ByteSpan(imageProcessor->mBlock.data(), imageProcessor->mBlock.size());

    CHIP_ERROR error = imageProcessor->ProcessHeader(block);
    if (error != CHIP_NO_ERROR)
    {
        iot_printf("Failed to process OTA image header\n");
        imageProcessor->mDownloader->EndDownload(error);
        return;
    }

    int8_t err = ota_write(block.data(), block.size());
    if (err != 0)
    {
        iot_printf("ota_write failed\n");
        imageProcessor->mDownloader->EndDownload(CHIP_ERROR_WRITE_FAILED);
        return;
    }
    imageProcessor->mParams.downloadedBytes += block.size();
    imageProcessor->mDownloader->FetchNextData();
}

CHIP_ERROR OTAImageProcessorImpl::SetBlock(ByteSpan & block)
{
    if (!IsSpanUsable(block))
    {
        ReleaseBlock();
        return CHIP_NO_ERROR;
    }
    if (mBlock.size() < block.size())
    {
        if (!mBlock.empty())
        {
            ReleaseBlock();
        }
        uint8_t * mBlock_ptr = static_cast<uint8_t *>(Platform::MemoryAlloc(block.size()));
        if (mBlock_ptr == nullptr)
        {
            return CHIP_ERROR_NO_MEMORY;
        }
        mBlock = MutableByteSpan(mBlock_ptr, block.size());
    }
    CHIP_ERROR err = CopySpanToMutableSpan(block, mBlock);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot copy block data: %" CHIP_ERROR_FORMAT, err.Format());
        return err;
    }
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ReleaseBlock()
{
    if (mBlock.data() != nullptr)
    {
        Platform::MemoryFree(mBlock.data());
    }
    mBlock = MutableByteSpan();
    return CHIP_NO_ERROR;
}

} // namespace chip
