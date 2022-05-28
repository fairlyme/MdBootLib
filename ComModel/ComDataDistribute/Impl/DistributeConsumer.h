#pragma once
#include "../Interface/IDistributeConsumer.h"

namespace MdLib {
	template <class DataType>
	class DistributeConsumer : public IDistributeConsumer<DataType>
	{
	private:
		bool _isEnable = false;
		std::string _productKey;
		std::shared_ptr<IDistributeContainer<DataType>> _container;

	public:
		DistributeConsumer(std::string productKey);
		// Í¨¹ý IDistributeConsumer ¼Ì³Ð
		virtual DistributeType GetDistributeType() override;
		virtual bool IsEnable() override;
		virtual void Enable() override;
		virtual void Disable() override;
		virtual std::string GetDistributeKey() override;
		virtual bool HasContainer() override;
		virtual std::shared_ptr<IProductContainer> GetContainer() override;
		virtual bool ReplaceContainer(std::shared_ptr<IProductContainer> container) override;
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() override;
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>> container) override;
	};

	template<class DataType>
	inline DistributeConsumer<DataType>::DistributeConsumer(std::string productKey)
		: _productKey(productKey)
	{

	}

	template<class DataType>
	inline DistributeType DistributeConsumer<DataType>::GetDistributeType()
	{
		return DistributeType::Consumer;
	}

	template<class DataType>
	inline bool DistributeConsumer<DataType>::IsEnable()
	{
		return _isEnable;
	}

	template<class DataType>
	inline void DistributeConsumer<DataType>::Enable()
	{
		_isEnable = true;
	}

	template<class DataType>
	inline void DistributeConsumer<DataType>::Disable()
	{
		_isEnable = false;
	}

	template<class DataType>
	inline std::string DistributeConsumer<DataType>::GetDistributeKey()
	{
		return _productKey;
	}

	template<class DataType>
	inline bool DistributeConsumer<DataType>::HasContainer()
	{
		if (_container) {
			return true;
		}
		return false;
	}

	template<class DataType>
	inline std::shared_ptr<IProductContainer> DistributeConsumer<DataType>::GetContainer()
	{
		return std::dynamic_pointer_cast<IProductContainer>(_container);
	}

	template<class DataType>
	inline bool DistributeConsumer<DataType>::ReplaceContainer(std::shared_ptr<IProductContainer> container)
	{
		auto actualContainer = std::dynamic_pointer_cast<IDistributeContainer<DataType>>(container);
		if (actualContainer) {
			return ReplaceDistributeContainer(actualContainer);
		}
		return false;
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

}
