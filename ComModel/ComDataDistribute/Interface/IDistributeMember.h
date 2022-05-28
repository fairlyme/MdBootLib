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
		virtual DistributeType GetDistributeType() = 0;

		virtual bool IsEnable() = 0;

		virtual void Enable() = 0;

		virtual void Disable() = 0;

		virtual std::string GetDistributeKey() = 0;

		virtual bool HasContainer() = 0;

		virtual std::shared_ptr<IProductContainer> GetContainer() = 0;

		virtual bool ReplaceContainer(std::shared_ptr<IProductContainer>) = 0;
	};
}