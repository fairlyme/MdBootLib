#pragma once
#include "../Interface/IDistributeProducer.h"
#include "VDistributeMember.h"
#include "../../ComDef/Errors/CustomErrors.h"

namespace MdLib {

	template<class DataType>
	class DistributeProductor : public IDistributeProductor<DataType>, public VDistributeMember
	{
	private:
		bool _isEnable = false;
		std::string _productKey;
		std::shared_ptr<IDistributeContainer<DataType>> _container;
		const int _MaxProductPipeLen = 10;

	protected:
		bool ApplyProduct(std::shared_ptr<DataType> product) {
			GetDistributeContainer()->Put(product);
		}

	public:
		DistributeProductor(std::string distributeKey, std::string productKey);

		// Í¨¹ý IDistributeProductor ¼Ì³Ð
		virtual std::string GetProductKey() override;
		virtual std::shared_ptr<MdLib::IDistributeContainer<DataType>> GetDistributeContainer() override;
		virtual bool ReplaceDistributeContainer(std::shared_ptr<MdLib::IDistributeContainer<DataType>>) override;

		// --- Impl VDistributeMember ---
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

		// === Impl VDistributeMember ===
	};


	template<class DataType>
	DistributeProductor<DataType>::DistributeProductor(std::string distributeKey, std::string productKey)
		: VDistributeMember(DistributeType::Producer, distributeKey),_productKey(productKey)
	{
	}


	template<class DataType>
	inline std::string DistributeProductor<DataType>::GetProductKey()
	{
		return _productKey;
	}


	template<class DataType>
	inline std::shared_ptr<MdLib::IDistributeContainer<DataType>> DistributeProductor<DataType>::GetDistributeContainer()
	{
		return _container;
	}

	template<class DataType>
	inline bool DistributeProductor<DataType>::ReplaceDistributeContainer(std::shared_ptr<MdLib::IDistributeContainer<DataType>> container)
	{
		_container = container;
		return true;
	}

}

