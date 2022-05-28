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

	template<class DataType>
	class IDistributeConsumer : public IDistributeMember {
	public:
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() = 0;
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>>) = 0;
	};
}