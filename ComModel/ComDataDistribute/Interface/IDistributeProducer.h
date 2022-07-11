#pragma once
// sys headers
#include <string>
#include <memory>

// module headers
#include "../ModuleCommon/ComDef.h"
#include "IDistributeContainer.h"	// ����

namespace MdLib {	
	template<class DataType>
	class IDistributeProductor {
	public:
		// ��ȡ��Ʒ����
		virtual std::string GetProductKey() = 0;

		// ��ȡ��������
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() = 0;

		// �滻��������
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>>) = 0;
	};
}