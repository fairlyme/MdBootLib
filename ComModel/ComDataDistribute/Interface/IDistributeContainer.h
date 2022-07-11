#pragma once
#include<string>
#include <memory>
#include <mutex>

#include "IProductContainer.h"

namespace MdLib {

	// 生产者和消费者模式容器
	template<class T>
	class IDistributeContainer : public IProductContainer {
	public:
		// 入队
		virtual bool Put(std::shared_ptr<T> item) = 0;

		// 出队
		virtual bool Take(std::shared_ptr<T>& item) = 0;


		// 等待出队 / 等待任意位置空置
		virtual void WaitDataDequeue(std::unique_lock<std::mutex> lk) = 0;
		virtual void WaitDataDequeue(std::unique_lock<std::mutex> lk, int ms) = 0;

		// 等待入队 / 等待任意数据
		virtual void WaitDataEnqueue(std::unique_lock<std::mutex> lk) = 0;
		virtual void WaitDataEnqueue(std::unique_lock<std::mutex> lk, int ms) = 0;
	};

}