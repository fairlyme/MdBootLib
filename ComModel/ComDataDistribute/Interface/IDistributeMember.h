#pragma once
// sys headers
#include <string>
#include <memory>

// module headers
#include "../ModuleCommon/ComDef.h"
#include "IProductContainer.h"

namespace MdLib {
	class IDistributeMember {
	public:
		// 获取调度Key
		virtual std::string GetDistributeKey() = 0;

		// 调度类型 生产/消费
		virtual DistributeType GetDistributeType() = 0;

		// 替换container
		virtual bool ReplaceContainer(std::shared_ptr<IProductContainer>) = 0;

		// 获取当前container
		virtual std::shared_ptr<IProductContainer> GetContainer() = 0;
		
		// has container
		virtual bool HasContainer() = 0;
	};
}