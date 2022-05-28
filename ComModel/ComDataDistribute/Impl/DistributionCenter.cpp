#include "DistributionCenter.h"
#include <assert.h>
#include <stdexcept>

namespace MdLib {
	void DistributionCenter::CheckContainerIsNeed(std::string productKey) {
		bool found = false;
		for (auto it = _producers.begin(); it != _producers.end(); it++) {
			if ((*it)->GetDistributeKey() == productKey) {
				found = true;
				break;
			}
		}

		if (!found) {
			std::lock_guard guard(_containerMtx);
			_containers.erase(productKey);

			for (auto it = _consumers.begin(); it != _consumers.end();) {
				if ((*it)->GetDistributeKey() == productKey) {
					(*it)->ReplaceContainer(nullptr);
					_consumers.erase(it);
				}
				else {
					it++;
				}
			}
		}
	}
	// 注册生产者
	bool DistributionCenter::RegistProducer(std::shared_ptr<IDistributeMember> producer, std::shared_ptr<IProductContainer> container) {
		if (producer->GetDistributeType() != DistributeType::Producer) {
			return false;
		}

		std::string productKey = container->GetProductKey();
		
		// 验证是否已存在
		auto containerIt = _containers.find(productKey);
		if (containerIt != _containers.end()) {
			// 存在
			if (containerIt->second.get() != container.get()) {
				// 非已存在的
				char errMsg[100] = { 0 };
				sprintf(errMsg,"key %s is already registed before.", productKey.c_str());
				throw std::logic_error(errMsg);
			}
		}

		// 检查是否已经包含
		if (producer->HasContainer()) {
			return false;
		}

		std::lock_guard guard(_producterMtx);

		// 设置 container
		if (!producer->ReplaceContainer(container)) {
			// 失败
			return false;
		}

		if (containerIt == _containers.end()) {
			_containers[productKey] = container;
		}

		_producers.emplace_back(producer);

		return true;
	}

	// 注册消费者
	bool DistributionCenter::RegistConsumer(std::shared_ptr<IDistributeMember> consumer) {
		if (consumer->GetDistributeType() != DistributeType::Consumer) {
			return false;
		}

		std::string productKey = consumer->GetDistributeKey();

		auto containerIt = _containers.find(productKey);

		if (containerIt == _containers.end()) {
			return false;
		}
		else
		{
			std::lock_guard guard(_consumerMtx);
			for (auto consumerIt = _consumers.begin(); consumerIt != _consumers.end(); consumerIt++) {
				if ((*consumerIt).get() == consumer.get()) {
					// 重复注册
					return false;
				}
			}

			if (!consumer->ReplaceContainer(containerIt->second)) {
				return false;
			}
			else
			{
				_consumers.emplace_back(consumer);
				return true;
			}
		}
	}

	// 注销生产者
	bool DistributionCenter::DeRegistProducer(std::shared_ptr<IDistributeMember> producer) {
		bool finded = false;
		for (auto it = _producers.begin(); it != _producers.end(); it++) {
			if (it->get() == producer.get()) {
				std::lock_guard guard(_producterMtx);
				producer->ReplaceContainer(nullptr);
				_producers.erase(it);
				finded = true;
			}
		}

		if (finded) {
			CheckContainerIsNeed(producer->GetDistributeKey());
			return true;
		}
		// 无匹配对象
		return false;
	}

	// 注销消费者
	bool DistributionCenter::DeRegistConsumer(std::shared_ptr<IDistributeMember> consumer) {
		bool finded = false;
		for (auto it = _consumers.begin(); it != _consumers.end(); it++) {
			if (it->get() == consumer.get()) {
				std::lock_guard guard(_consumerMtx);
				consumer->ReplaceContainer(nullptr);
				_consumers.erase(it);
				finded = true;
				break;
			}
		}

		return finded;
	}
}
