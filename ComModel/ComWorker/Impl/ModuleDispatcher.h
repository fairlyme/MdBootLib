#pragma once
// sys headers
#include <vector>
#include <condition_variable>
#include <mutex>

// lib headers 
#include "../ModuleComDef/ComDef.h"
#include "../../../ExternalDep/Singleton/Singleton.h"
#include "../../../ExternalDep/ThreadPool/thread_pool.hpp"

// module headers
#include "WorkerParamBase.h"
#include "Preset/ModuleWorker.h"
#include "../Interface/IDispatchable.h"

namespace MdLib {
	/// <summary>
	/// dispatch able obj
	/// </summary>
	class DispatchableObject : public IDispatchableObj {
		friend class ModuleDispatcher;

	protected:
		bool _managed = false;
		std::shared_ptr<IWorker> _iWorkerInvoke;

	protected:
		DispatchableObject(IWorker* workerInvokeObjRawPtr);

	public:
		DispatchableObject(std::shared_ptr<IWorker> wokerInvokeObj);

		~DispatchableObject();

		/// <summary>
		/// 获取包装的对象
		/// </summary>
		/// <returns></returns>
		virtual IWorker* GetWorker() override;

		virtual void DoProcess() override {
			OnPreProcess();
			_iWorkerInvoke->DoProcessOnce();
			OnFinishProcess();
		}
		
		virtual void OnPreProcess() override {
			_iWorkerInvoke->OnProcessStart();
		} 

		virtual void OnFinishProcess() override {
			_iWorkerInvoke->OnProcessStop();
		}
	};

	/// <summary>
	/// 非独立线程 worker
	///		非独立线程调度
	///		start -> 询问是否有工作 -> Plan -> Do
	/// </summary>
	class DependentDispatchObject : public DispatchableObject, public IDependent {
	private:
		std::mutex _stateMtx;
		bool _isPlaned = false;
		bool _runFlag = false;
		bool _isRunning = false;

	protected:
		DependentDispatchObject(IWorker* workerInvokeObjRawPtr);

	public:
		DependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj);

		virtual bool TryPlan() override;
		// 是否已经计划
		virtual bool IsPlaned() override;
		// 是否正在运行中
		virtual bool IsWorking() override;

		virtual void OnPreProcess() override;

		virtual void OnFinishProcess() override;
	};

	/// <summary>
	/// 非独立线程 worker
	///		调度
	///		StartAsync -> Stop
	/// </summary>
	class IndependentDispatchObject : public DispatchableObject, public IIndependent {
	protected:
		IndependentDispatchObject(IWorker* workerInvokeObjRawPtr);

	public:
		IndependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj);
		// 是否正在运行中
		virtual bool IsWorking() override {}
		// 启动 异步
		virtual bool StartAsync() override {}
		// 启动 同步
		virtual bool Start() override {}
		// 停止
		virtual bool Stop() override {}
	};


	class ModuleDispatcher : public Singleton<ModuleDispatcher>
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

	public:
		bool Init(int maxIndepThreadCnt, int maxDepThdCnt, int maxDepWorkerCnt);

		bool StopAll();

		void WaitAllTask();
	};
}

