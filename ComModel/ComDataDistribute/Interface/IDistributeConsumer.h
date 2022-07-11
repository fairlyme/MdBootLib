#pragma once

// sys headers
#include <memory>
#include <string>

// lib headers
#include "IDistributeContainer.h"

// module headers
#include "../ModuleCommon/ComDef.h"
#include "IDistributeMember.h" // 成员基类
#include "IDistributeContainer.h" // 容器

namespace MdLib {
	/*
		分布式 消费者
	*/
	template<class DataType>
	class IDistributeConsumer {
	public:
		// 获取目标产品的名称
		virtual std::string GetTargetProductKey() = 0;
		// 获取分发容器
		virtual std::shared_ptr<IDistributeContainer<DataType>> GetDistributeContainer() = 0;
		// 替换新的分发容器
		virtual bool ReplaceDistributeContainer(std::shared_ptr<IDistributeContainer<DataType>>) = 0;
		
		// 检查需要执行
		virtual bool NeedTakeProduct() = 0;
		virtual bool Consume(DataType* item) = 0;
		virtual bool TryTakeProduct(std::shared_ptr<DataType>& item) = 0;
		virtual bool WaitProduct(int ms) = 0;
		virtual bool IsConsumeDone() = 0;
	};
}