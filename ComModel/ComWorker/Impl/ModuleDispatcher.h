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
		/// ��ȡ��װ�Ķ���
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
	/// �Ƕ����߳� worker
	///		�Ƕ����̵߳���
	///		start -> ѯ���Ƿ��й��� -> Plan -> Do
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
		// �Ƿ��Ѿ��ƻ�
		virtual bool IsPlaned() override;
		// �Ƿ�����������
		virtual bool IsWorking() override;

		virtual void OnPreProcess() override;

		virtual void OnFinishProcess() override;
	};

	/// <summary>
	/// �Ƕ����߳� worker
	///		����
	///		StartAsync -> Stop
	/// </summary>
	class IndependentDispatchObject : public DispatchableObject, public IIndependent {
	protected:
		IndependentDispatchObject(IWorker* workerInvokeObjRawPtr);

	public:
		IndependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj);
		// �Ƿ�����������
		virtual bool IsWorking() override {}
		// ���� �첽
		virtual bool StartAsync() override {}
		// ���� ͬ��
		virtual bool Start() override {}
		// ֹͣ
		virtual bool Stop() override {}
	};


	class ModuleDispatcher : public Singleton<ModuleDispatcher>
	{
		friend class Singleton<ModuleDispatcher>;

	private:
		ModuleDispatcher();
		bool _allFinish;
		std::condition_variable _allFinishCv;	

		// �Ƕ����߳�worker
		std::vector<std::shared_ptr<DependentDispatchObject>> _allDependentWorker;
		// �����߳� worker
		std::vector<std::shared_ptr<IndependentDispatchObject>> _allInDependentWorker;

		// ���Ƕ�����������, eg �Ƕ����߳����� x 2
		int _maxDependentWorkerCnt;

		// ���Ƕ����߳�
		int _maxDependentThreadCnt;

		// �������߳�
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

