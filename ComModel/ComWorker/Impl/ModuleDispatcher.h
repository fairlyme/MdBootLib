#pragma once
// sys headers
#include <vector>
#include <condition_variable>

// lib headers 
#include "../../../ExternalDep/Singleton/Singleton.h"
#include "../../../ExternalDep/ThreadPool/thread_pool.hpp"

// module headers
#include "WorkerParamBase.h"
#include "Preset/ModuleWorker.h"
#include "Preset/ThreadWorker.h" // 单独运行
#include "DependentDispatchObject.h"
#include "IndependentDispatchObject.h"

namespace MdLib {

	class ModuleDispatcher : public Singleton<ModuleDispatcher>,protected ThreadWorkerBase
	{
		friend class Singleton<ModuleDispatcher>;

	private:
		ModuleDispatcher();
		bool _allFinish;
		std::condition_variable _allFinishCv;	

		// 非独立线程worker
		std::vector<std::shared_ptr<DependentDispatchObject>> _allDependentWorker;
		// 独立线程 worker
		std::vector<std::shared_ptr<IndependentDispatchObject>> _allInDependentWorker;

		// 最大非独立工作进程, eg 非独立线程数量 x 2
		int _maxDependentWorkerCnt;

		// 最大非独立线程
		int _maxDependentThreadCnt;

		// 最大独立线程
		int _maxIndependentThreadCnt;

		thread_pool* _threadPool;
		
	private:

		std::thread _dispatcherThread;
		std::mutex _dispatcherMtx;
		bool _runFlag;

	protected:
		bool _ApplyDependentWorker(std::shared_ptr<DependentDispatchObject> worker);

		bool _ApplyIndependetWorker(std::shared_ptr<IndependentDispatchObject> worker);

	protected:
		// 通过 ThreadWorkerBase 继承
		virtual void DoProcessOnce() override;

		virtual void OnProcessStop() override;

		virtual void OnProcessStart() override;

		virtual bool CanProcess() override;

		virtual bool IsProcessAllDone() override;

		virtual std::string What() override;

	public:
		bool Init(int maxIndepThreadCnt, int maxDepThdCnt, int maxDepWorkerCnt);

		bool StartWorker(std::string moduleName, WorkerType dispatchType, IModuleParam* mdParam);

		bool StartWorker(configor::json& configJsonObj);

		bool DispatchWorker(std::shared_ptr<IWorker> worker, WorkerType dispatchType);

		bool StopAll();

		void WaitAllTask();

		// 测试代码 完成后删除
		void TestStart() {
			StartProcessAsync();
		}
	};
}

