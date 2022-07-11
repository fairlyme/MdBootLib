#pragma once
#include "../Interface/IDistributeConsumer.h"
#include "VDistributeMember.h"
#include "../../ComDef/Errors/CustomErrors.h"

namespace MdLib {
	template <class DataType>
	class DistributeConsumer : public IDistributeConsumer<DataType>,public VDistributeMember
	{
	private:
		std::shared_ptr<IDistributeContainer<DataType>> _container;
		std::mutex _waitLk;
		std::string _productKey;
	protected:
		std::shared_ptr<DataType> _productToConsume;

	public:
		DistributeConsumer(std::string distributeKey, std::string productKey);

		// 通过 IDistributeConsumer 继承
		virtual std::string GetTargetProductKey() override;

		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() override;

		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>> container) override;

		// 通过 VDistributeMember 继承
		virtual bool IsTargetContainer(std::shared_ptr<IProductContainer> container) override {
			auto actualContainer = std::dynamic_pointer_cast<IDistributeContainer<DataType>>(container);

			if (actualContainer) {
				return true;
			}

			return false;
		}
		virtual void OnContainerChanged(std::shared_ptr<IProductContainer> container) override {
			if (container) {
				auto actualContainer = std::dynamic_pointer_cast<IDistributeContainer<DataType>>(container);
				if (actualContainer) {
					_container = actualContainer;
				}
				else {
					throw InvalidParamErr("invalid container type got.");
				}
			}
			else
			{
				_container = nullptr;
			}
		}

	public:

		void DoConsume();

		virtual bool NeedTakeProduct() override {
			return !_productToConsume;
		}

		virtual bool TryTakeProduct(std::shared_ptr<DataType>& item) final {
			return _container->Take(item);
		}

		virtual bool WaitProduct(int ms) final {
			_container->WaitDataEnqueue(std::unique_lock<std::mutex>(_waitLk), ms);
			return !_container->IsEmpty();
		}

		// To impl
		virtual bool Consume(DataType* product) = 0;
		virtual bool IsConsumeDone() = 0;
	};

	template<class DataType>
	inline DistributeConsumer<DataType>::DistributeConsumer(std::string distributeKey, std::string productKey)
		: VDistributeMember(DistributeType::Consumer, distributeKey),_productKey(productKey)
	{

	}

	template<class DataType>
	inline std::string DistributeConsumer<DataType>::GetTargetProductKey()
	{
		return _productKey;
	}


	template<class DataType>
	inline std::shared_ptr<IDistributeContainer<DataType>> DistributeConsumer<DataType>::GetDistributeContainer()
	{
		return _container;
	}

	template<class DataType>
	inline bool DistributeConsumer<DataType>::ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>> container)
	{
		_container = container;
		return true;
	}

	template<class DataType>
	inline void DistributeConsumer<DataType>::DoConsume(){
			if (NeedTakeProduct()) {
				if (!TryTakeProduct(_productToConsume)) {
					WaitProduct(100);
					return;
				}
				else
				{
					return;
				}
			}
			else {
				if (Consume(_productToConsume.get())) {
					_productToConsume.reset();
				}
				return;
			}
	}
}
