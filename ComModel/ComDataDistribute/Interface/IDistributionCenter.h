#pragma once
// sys headers
#include <memory>
#include <string>

// lib headers
#include <ComModel/ComDef/ComDef.h>

// module headers
#include "IDistributeMember.h"
#include "IDistributeContainer.h"

namespace MdLib {
	class IDistributionCenter {
	public:
		// ע��������
		virtual bool RegistProducer(std::shared_ptr<IDistributeMember> producer, std::shared_ptr<IProductContainer> container) = 0;

		// ע��������
		virtual bool DeRegistProducer(std::shared_ptr<IDistributeMember> producer) = 0;
		virtual bool DeRegistProducer(IDistributeMember* producer) = 0;


		// ע��������
		virtual bool RegistConsumer(std::shared_ptr<IDistributeMember> comsumer) = 0;

		// ע��������
		virtual bool DeRegistConsumer(std::shared_ptr<IDistributeMember> comsumer) = 0;
		virtual bool DeRegistConsumer(IDistributeMember* comsumer) = 0;
	};
}