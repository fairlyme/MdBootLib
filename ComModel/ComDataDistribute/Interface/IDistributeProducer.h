#pragma once
// sys headers
#include <string>
#include <memory>

// module headers
#include "../ModuleCommon/ComDef.h"
#include "IDistributeMember.h"		// 成员基类
#include "IDistributeContainer.h"	// 容器

namespace MdLib {
	template<class DataType>
	class IDistributeProducer : public IDistributeMember {
	public:
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() = 0;
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>>) = 0;
	};
}