#pragma once
// sys headers
#include <string>
#include <memory>

// module headers
#include "../ModuleCommon/ComDef.h"
#include "IDistributeContainer.h"	// 容器

namespace MdLib {	
	template<class DataType>
	class IDistributeProductor {
	public:
		// 获取产品名称
		virtual std::string GetProductKey() = 0;

		// 获取调度容器
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() = 0;

		// 替换调度容器
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>>) = 0;
	};
}