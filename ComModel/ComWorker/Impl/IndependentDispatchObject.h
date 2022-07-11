#pragma once
#include "DispatchableObject.h"

namespace MdLib {
	/// <summary>
	/// 非独立线程 worker
	///		调度
	///		StartAsync -> Stop
	/// </summary>
	class IndependentDispatchObject : public DispatchableObject, public IIndependent {
	protected:
		std::mutex _stateMtx;
		bool _isRunning = false;
		bool _runningFlag = false;

		std::thread _workThd;
		IndependentDispatchObject(IWorker* workerInvokeObjRawPtr);

		void _DoProcessWarp();

	public:
		// 独立调度obj
		IndependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj);
		// 正在工作中
		virtual bool IsWorking() override;
		// 启动 异步
		virtual bool StartAsync() override;
		// 启动 同步
		virtual bool Start() override;
		// 停止
		virtual bool Stop(bool wait) override;
		// 等待线程结束
		virtual void Join() override;
	};

}
