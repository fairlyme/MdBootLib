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
	// ע��������
	bool DistributionCenter::RegistProducer(std::shared_ptr<IDistributeMember> producer, std::shared_ptr<IProductContainer> container) {
		if (producer->GetDistributeType() != DistributeType::Producer) {
			return false;
		}

		std::string productKey = container->GetProductKey();
		
		// ��֤�Ƿ��Ѵ���
		auto containerIt = _containers.find(productKey);
		if (containerIt != _containers.end()) {
			// ����
			if (containerIt->second.get() != container.get()) {
				// ���Ѵ��ڵ�
				char errMsg[100] = { 0 };
				sprintf(errMsg,"key %s is already registed before.", productKey.c_str());
				throw std::logic_error(errMsg);
			}
		}

		// ����Ƿ��Ѿ�����
		if (producer->HasContainer()) {
			return false;
		}

		std::lock_guard guard(_producterMtx);

		// ���� container
		if (!producer->ReplaceContainer(container)) {
			// ʧ��
			return false;
		}

		if (containerIt == _containers.end()) {
			_containers[productKey] = container;
		}

		_producers.emplace_back(producer);

		return true;
	}

	// ע��������
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
					// �ظ�ע��
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

	// ע��������
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
		// ��ƥ�����
		return false;
	}

	// ע��������
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
