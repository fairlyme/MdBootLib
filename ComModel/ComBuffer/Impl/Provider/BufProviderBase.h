#pragma once
#include <string>
#include "../../Interface/IBufProvider.h"
#include "../BufDev.h"

namespace MdLib {
	/// <summary>
	/// CpuBuffer ����ʵ��
	///		ʵ�ַ�������Cpu��buffer 
	///		�����û����
	/// </summary>
	class BufProviderBase : public IBufProvider
	{
		BufDev* _bufDev;
	protected:
		void SetDevDataTypeUnitLen(BufferDataType type, int len);
	public:
		BufProviderBase(std::string devKey);
		~BufProviderBase();

		// ͨ�� IBufProvider �̳�
		virtual IBufDev* GetDevice() override;
		// virtual void* CreateBuf(IBufParam* param) override;
		// virtual void ReleaseBuf(void* bufPtr, IBufParam* param) override;
		// virtual bool CopyDevToCpu(void* src, void* dst, IBufCopyParam* copyParam) override;
		// virtual bool CopyCpuToDev(void* src, void* dst, IBufCopyParam* copyParam) override;
		// virtual bool CopyDevToDev(void* src, void* dst, IBufCopyParam* copyParam) override;
	};
}