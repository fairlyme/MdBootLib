#pragma once
// sys headers
#include <string>
#include <mutex>
#include <thread>

// lib headers
#include "../../ModuleComDef/ComDef.h"
#include "../../../ComDef/Errors/CustomErrors.h"

// module headers
#include "../../Interface/IWorker.h"

namespace MdLib {
	class ThreadWorkerBase : public IWorker
	{
	private:
		std::mutex _stateMutex;
		std::thread _processThread;
		bool _isRunning{ false };
		bool _runFlag{ false };
		bool _isStarted{ false };
		bool _isThreadCreated{ false };

	protected:
		void _DoProcessWarp();

		void ProcessExit();
	public:
		ThreadWorkerBase();

		bool IsProcessExit();

		virtual bool IsWorking();

		bool StartProcess();

		bool StartProcessAsync();

		void Join();

		void StopProcess();
	};


	class ThreadWorker : public ThreadWorkerBase{
	private:
		std::shared_ptr<IWorker> _workerInvokeObj;

	protected:

	public:
		ThreadWorker(std::shared_ptr<IWorker> workerInvokeObj);

		// Í¨¹ý ThreadWorkerBase ¼Ì³Ð
		virtual void DoProcessOnce() override;

		virtual void OnProcessStop() override;

		virtual void OnProcessStart() override;

		virtual bool IsProcessAllDone() override;

		virtual std::string What() override;
	};
}