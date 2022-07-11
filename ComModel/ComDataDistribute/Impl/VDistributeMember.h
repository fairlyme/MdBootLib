#pragma once

#include "../Interface/IDistributeProducer.h"

namespace MdLib {
	class VDistributeMember : public IDistributeMember {
	private:
		std::string  _distributeKey;
		std::shared_ptr<IProductContainer> _productPipe;

	public:
		const DistributeType MemberType;

		VDistributeMember(DistributeType memberType, std::string distributeKey) :
			 _distributeKey(distributeKey),MemberType(memberType)
		{

		}


		// 获取调度Key
		virtual std::string GetDistributeKey() override{
			return _distributeKey;
		}

		// 调度类型 生产/消费
		virtual DistributeType GetDistributeType() override {
			return MemberType;
		}


		// 替换container
		virtual bool ReplaceContainer(std::shared_ptr<IProductContainer> container) override {
			if (IsTargetContainer(container)) {
				_productPipe = container;
				OnContainerChanged(container);
			}
			return true;
		}

		// has container
		virtual bool HasContainer() override {
			if (_productPipe) {
				return true;
			}
			return false;
		}

		// 获取当前container
		virtual std::shared_ptr<IProductContainer> GetContainer() override {
			return _productPipe;
		}


		// --- to impl ---
		virtual bool IsTargetContainer(std::shared_ptr<IProductContainer> container) = 0;

		virtual void OnContainerChanged(std::shared_ptr<IProductContainer> container) = 0;
		// === to impl ===
	};
}