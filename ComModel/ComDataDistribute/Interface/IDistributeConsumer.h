#pragma once

// sys headers
#include <memory>
#include <string>

// lib headers
#include "IDistributeContainer.h"

// module headers
#include "../ModuleCommon/ComDef.h"
#include "IDistributeMember.h" // ��Ա����
#include "IDistributeContainer.h" // ����

namespace MdLib {
	/*
		�ֲ�ʽ ������
	*/
	template<class DataType>
	class IDistributeConsumer {
	public:
		// ��ȡĿ���Ʒ������
		virtual std::string GetTargetProductKey() = 0;
		// ��ȡ�ַ�����
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() = 0;
		// �滻�µķַ�����
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>>) = 0;
		
		// �����Ҫִ��
		virtual bool NeedTakeProduct() = 0;
		virtual bool Consume(DataType* item) = 0;
		virtual bool TryTakeProduct(std::shared_ptr<DataType>& item) = 0;
		virtual bool WaitProduct(int ms) = 0;
		virtual bool IsConsumeDone() = 0;
	};
}