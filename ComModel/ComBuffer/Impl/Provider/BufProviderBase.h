#pragma once
#include <string>
#include "../../Interface/IBufProvider.h"
#include "../BufDev.h"

namespace MdLib {
	/// <summary>
	/// CpuBuffer 生产实现
	///		实现分批管理Cpu的buffer 
	///		不采用缓存池
	/// </summary>
	class BufProviderBase : public IBufProvider
	{
		BufDev* _bufDev;
	protected:
		void SetDevDataTypeUnitLen(BufferDataType type, int len);
	public:
		BufProviderBase(std::string devKey);
		~BufProviderBase();

		// 通过 IBufProvider 继承
		virtual IBufDev* GetDevice() override;
		// virtual void* CreateBuf(IBufParam* param) override;
		// virtual void ReleaseBuf(void* bufPtr, IBufParam* param) override;
		// virtual bool CopyDevToCpu(void* src, void* dst, IBufCopyParam* copyParam) override;
		// virtual bool CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam) override;
		// virtual bool CopyDevToDev(void* src, void* dst, IBufCopyParam* copyParam) override;
	};
}