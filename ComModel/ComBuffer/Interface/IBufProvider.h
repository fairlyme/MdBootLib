#pragma once

#include "IBufCopyParam.h"
#include "IBufDev.h"

namespace MdLib {
	class IBufProvider {
	public:
		// 获取设备
		virtual IBufDev* GetDevice() = 0;

		// 创建 buffer
		virtual void* CreateBuf(BufferDataType dataType, int unitsCount) = 0;

		// 释放 buffer
		virtual void ReleaseBuf(void*& bufPtr,int bufSize) = 0;

		// 设备到设备的cpu
		virtual bool CopyDevToCpu(void* src,void* dst, IBufCopyParam* copyParam) = 0;

		// cpu到设备的拷贝
		virtual bool CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam) = 0;
	};
}