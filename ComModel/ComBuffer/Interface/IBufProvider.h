#pragma once

#include "IBufCopyParam.h"
#include "IBufDev.h"

namespace MdLib {
	class IBufProvider {
	public:
		// ��ȡ�豸
		virtual IBufDev* GetDevice() = 0;

		// ���� buffer
		virtual void* CreateBuf(BufferDataType dataType, int unitsCount) = 0;

		// �ͷ� buffer
		virtual void ReleaseBuf(void*& bufPtr,int bufSize) = 0;

		// �豸���豸��cpu
		virtual bool CopyDevToCpu(void* src,void* dst, IBufCopyParam* copyParam) = 0;

		// cpu���豸�Ŀ���
		virtual bool CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam) = 0;
	};
}