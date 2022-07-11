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
#include "Preset/ThreadWorker.h" // ��������
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

	protected:
		bool _ApplyDependentWorker(std::shared_ptr<DependentDispatchObject> worker);

		bool _ApplyIndependetWorker(std::shared_ptr<IndependentDispatchObject> worker);

	protected:
		// ͨ�� ThreadWorkerBase �̳�
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

		// ���Դ��� ��ɺ�ɾ��
		void TestStart() {
			StartProcessAsync();
		}
	};
}

