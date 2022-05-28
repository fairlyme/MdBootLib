#include "CpuBufProvider.h"

#include <cstring>

#include "../../../ComDef/Errors/CustomErrors.h"

namespace MdLib {

	void* CpuBufProvider::CreateBuf(BufferDataType dataType, int unitsCount)
	{
		// throw NotImplErr();
		int totalLen = GetDevice()->GetUnitLenOfDataType(dataType)* unitsCount;
		
		return malloc(totalLen);
	}
	void CpuBufProvider::ReleaseBuf(void*& bufPtr, int bufSize)
	{
		free(bufPtr);
		bufPtr = nullptr;
	}

	bool CpuBufProvider::CopyDevToCpu(void* src, void* dst, IBufCopyParam* copyParam)
	{
		memcpy(dst + copyParam->DstOffset(), src + copyParam->SrcOffset(), copyParam->CopyLen());
		return true;
	}
	bool CpuBufProvider::CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam)
	{
		memcpy(dst + copyParam->DstOffset(), src + copyParam->SrcOffset(), copyParam->CopyLen());
		return true;
	}

}