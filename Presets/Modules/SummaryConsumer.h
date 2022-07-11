#pragma once
#include <type_traits>

#include "../../ComModel/ComDataDistribute/Impl/DistributeConsumer.h"
#include "../../ComModel/ComModule/ComModule.h"
#include "../../ComModel/ComDef/ComBase/ISummariable.h"



namespace MdLib {
	// ²ÎÊý
	class SummaryConsumerParam : public ModuleParamBase{
		
	};

	template<class _DataType>
	class SummaryConsumer: 
		public DistributeConsumer<_DataType>,
		public Module<SummaryConsumerParam>
	{
	public:
		SummaryConsumer(std::string productName) : DistributeConsumer<_DataType>(productName) {
			static_assert(std::is_base_of_v<ISummariable, _DataType>);
		}

		virtual bool Consume(_DataType* item) override {
			return true;
		}

		virtual bool IsConsumeDone() override {
			return false;
		}

		// module 
		virtual bool Init(SummaryConsumerParam* param) override {
			return true;
		}
	};
}

