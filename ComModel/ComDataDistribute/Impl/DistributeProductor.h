#pragma once
#include "../Interface/IDistributeProducer.h"

namespace MdLib {
	template<class DataType>
	class DistributeProductor : public IDistributeProducer<DataType>
	{
	private:
		bool _isEnable = false;
		std::string _productKey;
		std::shared_ptr<IDistributeContainer<DataType>> _container;

	protected:
		bool ApplyProduct(std::shared_ptr<DataType> product) {
			GetDistributeContainer()->Put(product);
		}

	public:
		DistributeProductor(std::string productKey);
		// Í¨¹ý IDistributeProducer ¼Ì³Ð
		virtual DistributeType GetDistributeType() override;
		virtual bool IsEnable() override;
		virtual void Enable() override;
		virtual void Disable() override;
		virtual std::string GetDistributeKey() override;
		virtual bool HasContainer() override;
		virtual std::shared_ptr<MdLib::IProductContainer> GetContainer() override;
		virtual bool ReplaceContainer(std::shared_ptr<MdLib::IProductContainer>) override;
		virtual std::shared_ptr<MdLib::IDistributeContainer<DataType>> GetDistributeContainer() override;
		virtual bool ReplaceDistributeContainer(std::shared_ptr<MdLib::IDistributeContainer<DataType>>) override;
	};


	template<class DataType>
	inline DistributeProductor<DataType>::DistributeProductor(std::string productKey) : _productKey(productKey) {}

	template<class DataType>
	inline DistributeType DistributeProductor<DataType>::GetDistributeType()
	{
		return DistributeType::Producer;
	}

	template<class DataType>
	inline bool DistributeProductor<DataType>::IsEnable()
	{
		return _isEnable;
	}

	template<class DataType>
	inline void DistributeProductor<DataType>::Enable()
	{
		_isEnable = true;
	}

	template<class DataType>
	inline void DistributeProductor<DataType>::Disable()
	{
		_isEnable = false;
	}

	template<class DataType>
	inline std::string DistributeProductor<DataType>::GetDistributeKey()
	{
		return _productKey;
	}

	template<class DataType>
	inline bool DistributeProductor<DataType>::HasContainer()
	{
		if (_container) {
			return true;
		}
		return false;
	}

	template<class DataType>
	inline std::shared_ptr<IProductContainer> DistributeProductor<DataType>::GetContainer()
	{
		return std::dynamic_pointer_cast<IProductContainer>(_container);
	}

	template<class DataType>
	inline bool DistributeProductor<DataType>::ReplaceContainer(std::shared_ptr<MdLib::IProductContainer> container)
	{
		auto actualContainer = std::dynamic_pointer_cast<IDistributeContainer<DataType>>(container);
		if (actualContainer) {
			return ReplaceDistributeContainer(actualContainer);
		}
		return false;
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

