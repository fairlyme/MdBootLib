#pragma once
#include "BufProviderBase.h"

#include "../../../ComDef/ComDef.h"

namespace MdLib {
	/// <summary>
	/// CpuBuffer 生产实现
	///		实现分批管理Cpu的buffer 
	///		不采用缓存池
	/// </summary>
	class CpuBufProvider : public BufProviderBase
	{
		
	public:
		CpuBufProvider() : BufProviderBase(CPU_DEV_NAME){
			// TODO : 更新
			// SetDevDataTypeUnitLen(BufferDataType::INT_16,2);
		}

		// 通过 IBufProvider 继承
		virtual void* CreateBuf(BufferDataType dataType, int unitsCount) override;
		virtual void ReleaseBuf(void*& bufPtr,int bufSize) override;
		virtual bool CopyDevToCpu(void* src, void* dst, IBufCopyParam* copyParam) override;
		virtual bool CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam) override;
	};
}
