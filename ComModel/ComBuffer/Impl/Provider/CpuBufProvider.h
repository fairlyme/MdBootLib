#pragma once
#include "BufProviderBase.h"

#include "../../../ComDef/ComDef.h"

namespace MdLib {
	/// <summary>
	/// CpuBuffer ����ʵ��
	///		ʵ�ַ�������Cpu��buffer 
	///		�����û����
	/// </summary>
	class CpuBufProvider : public BufProviderBase
	{
		
	public:
		CpuBufProvider() : BufProviderBase(CPU_DEV_NAME){
			// TODO : ����
			// SetDevDataTypeUnitLen(BufferDataType::INT_16,2);
		}

		// ͨ�� IBufProvider �̳�
		virtual void* CreateBuf(BufferDataType dataType, int unitsCount) override;
		virtual void ReleaseBuf(void*& bufPtr,int bufSize) override;
		virtual bool CopyDevToCpu(void* src, void* dst, IBufCopyParam* copyParam) override;
		virtual bool CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam) override;
	};
}
