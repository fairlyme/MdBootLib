#pragma once
// sys header
#include <unordered_map>
#include <list>

#include <mutex>

#include "ComModel/ComDef/ComDef.h"
#include "ExternalDep/Singleton/Singleton.h"

#include "../Interface/IDistributeProducer.h"
#include "../Interface/IDistributeConsumer.h"
#include "../Interface/IDistributionCenter.h"
#include "../Interface/IDistributeContainer.h"

#include "DistributionContainer.h"



namespace MdLib {
	class DistributionCenter : public IDistributionCenter ,public Singleton<DistributionCenter>{
		friend class Singleton<DistributionCenter>;
	private:
		std::mutex _containerMtx;
		std::unordered_map<std::string, std::shared_ptr<IProductContainer>> _containers;

		std::mutex _producterMtx;
		std::list<std::shared_ptr<IDistributeMember>> _producers;

		std::mutex _consumerMtx;
		std::list<std::shared_ptr<IDistributeMember>> _consumers;

	protected:
		void CheckContainerIsNeed(std::string productKey);

	private:
		DistributionCenter(){}
	public:
		template<class T>
		bool RegistDistributeProducer(std::shared_ptr<IDistributeProducer<T>> producer);

		template<class T>
		bool DeRegistDistributeProducer(std::shared_ptr<IDistributeProducer<T>> producer);

		template<class T>
		bool RegistDistributeConsumer(std::shared_ptr<IDistributeConsumer<T>> consumer);

		template<class T>
		bool DeRegistDistributeConsumer(std::shared_ptr<IDistributeConsumer<T>> consumer);

		// 注册生产者
		virtual bool RegistProducer(std::shared_ptr<IDistributeMember> producer, std::shared_ptr<IProductContainer> container) override;

		// 注册消费者
		virtual bool RegistConsumer(std::shared_ptr<IDistributeMember> consumer) override;

		// 注销生产者
		virtual bool DeRegistProducer(std::shared_ptr<IDistributeMember> producer) override;

		// 注销消费者
		virtual bool DeRegistConsumer(std::shared_ptr<IDistributeMember> comsumer) override;
	};

	template<class T>
	bool DistributionCenter::RegistDistributeProducer(std::shared_ptr<IDistributeProducer<T>> producer)
	{
		std::string productKey = producer->GetDistributeKey();

		auto containerIt = _containers.find(productKey);

		if (containerIt == _containers.end()) {
			// cretae container
			std::shared_ptr<DistributionContainer<T>> container = std::make_shared<DistributionContainer<T>>(productKey,10);

			return RegistProducer(std::dynamic_pointer_cast<IDistributeMember>(producer), std::dynamic_pointer_cast<IProductContainer>(container));
		}
		else
		{
			return RegistProducer(std::dynamic_pointer_cast<IDistributeMember>(producer), containerIt->second);
		}
	}


	template<class T>
	bool DistributionCenter::DeRegistDistributeProducer(std::shared_ptr<IDistributeProducer<T>> producer) {
		return DeRegistProducer(std::dynamic_pointer_cast<IDistributeMember>(producer));
	}

	template<class T>
	bool DistributionCenter::RegistDistributeConsumer(std::shared_ptr<IDistributeConsumer<T>> consumer)
	{
#if DEBUG
		std::shared_ptr<IDistributeMember> consumerCast = std::dynamic_pointer_cast<IDistributeMember>(consumer);
		bool ret = RegistConsumer(consumerCast);
		return ret;
#else
		return RegistConsumer(std::dynamic_pointer_cast<IDistributeMember>(consumer));
#endif
	}

	template<class T>
	bool DistributionCenter::DeRegistDistributeConsumer(std::shared_ptr<IDistributeConsumer<T>> consumer) {
		return DeRegistConsumer(std::dynamic_pointer_cast<IDistributeMember>(consumer));
	}
}