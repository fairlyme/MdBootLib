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
		// ��ȡ����Key
		virtual std::string GetDistributeKey() = 0;

		// �������� ����/����
		virtual DistributeType GetDistributeType() = 0;

		// �滻container
		virtual bool ReplaceContainer(std::shared_ptr<IProductContainer>) = 0;

		// ��ȡ��ǰcontainer
		virtual std::shared_ptr<IProductContainer> GetContainer() = 0;
		
		// has container
		virtual bool HasContainer() = 0;
	};
}