#include "Buffer.h"
#include "../Interface/IBuf.h"

#include "BufferManager.h"

#include "../../ComDef/Errors/CustomErrors.h"

namespace MdLib {

    Buffer::Buffer(std::string dev, BufferDataType dataType, int bufUnits)
        : devKey(dev), devPtr(nullptr)
    {
        _bufHolded = std::move(BufferManageCenter::getInstance().CreateBuf(dev, dataType, bufUnits));
    }

    Buffer::~Buffer()
    {
        void* bufRawPtr = _bufHolded->GetBuffer();
        BufferManageCenter::getInstance().GetProvider(devKey)->ReleaseBuf(bufRawPtr, _bufHolded->GetBufferBytes());
    }

    IBuf* Buffer::GetBuffer()
    {
        return _bufHolded.get();
    }

    IBufDev* Buffer::GetDevice()
    {
        if (devPtr == nullptr) {
            BufferManageCenter::getInstance().TryGetDev(devKey, devPtr);
        }
        return devPtr;
    }

    std::string Buffer::GetDeviceKey()
    {
        return devKey;
    }

    bool Buffer::ToDev(std::string devKey)
    {
        return false;
    }

    bool Buffer::ToCpu()
    {
        return ToDev(CPU_DEV_NAME);
    }

    bool Buffer::CopyTo(IBufContainer* buf)
    {
        throw NotImplErr();

        if (devKey == CPU_DEV_NAME) {
            IBufProvider* provider = BufferManageCenter::getInstance().GetProvider(buf->GetDeviceKey());

            if (provider == nullptr) {
                return false;
            }

            // provider->CopyCpuToDev(GetBuffer()->GetBuffer(),buf->GetBuffer()->GetBuffer(),)
        }
        return false;
    }
};