#include "ThreadWorker.h"

namespace MdLib {
	void ThreadWorkerBase::_DoProcessWarp() {
		OnProcessStart();
		{
			std::lock_guard<std::mutex> guard(_stateMutex);
			if (_isRunning) {
				return;
			}
			_isRunning = true;
		}
		while (_runFlag)
		{
			DoProcessOnce();

			if (IsProcessAllDone()) {
				std::lock_guard<std::mutex> guard(_stateMutex);
				_runFlag = false;
			}
		}
		{
			std::lock_guard<std::mutex> guard(_stateMutex);
			_isRunning = false;
			OnProcessStop();
		}
	}

	void ThreadWorkerBase::ProcessExit() {
		_runFlag = false;
	}

	ThreadWorkerBase::ThreadWorkerBase() {

	}

	bool ThreadWorkerBase::IsProcessExit() {
		return !_runFlag;
	}

	bool ThreadWorkerBase::IsWorking() { return _isRunning; }

	bool ThreadWorkerBase::StartProcess() {
		if (_isStarted || _isRunning) {
			return false;
		}
		_isStarted = true;
		// OnProcessStart();
		_DoProcessWarp();
		return true;
	}

	bool ThreadWorkerBase::StartProcessAsync() {
		if (_isStarted || _isRunning) {
			return false;
		}

		_isStarted = true;
		_runFlag = true;

		_isThreadCreated = true;
		_processThread = std::thread(&ThreadWorkerBase::_DoProcessWarp, this);

		pthread_setname_np(_processThread.native_handle(), What().c_str());
		return true;
	}

	void ThreadWorkerBase::Join() {
		if (!_isThreadCreated) {
			return;
		}
		if (_processThread.joinable()) {
			_processThread.join();
		}
		else
		{
			_processThread.detach();
		}
	}

	void ThreadWorkerBase::StopProcess() {
		if (!_isStarted) {
			// 标记Stop已经处理过了
			_isStarted = false;
		}
		_runFlag = false;

		if (_isThreadCreated) {
			Join();
			_isThreadCreated = false;
		}

		// 调用事件
		OnProcessStop();
	}
	ThreadWorker::ThreadWorker(std::shared_ptr<IWorker> workerInvokeObj)
		: _workerInvokeObj(workerInvokeObj)
	{
	}
	void ThreadWorker::DoProcessOnce()
	{
		_workerInvokeObj->DoProcessOnce();
	}
	void ThreadWorker::OnProcessStop()
	{
		_workerInvokeObj->OnProcessStop();
	}
	void ThreadWorker::OnProcessStart()
	{
		_workerInvokeObj->OnProcessStart();
	}
	
	bool ThreadWorker::IsProcessAllDone() {
		return _workerInvokeObj->IsProcessAllDone();
	}
	
	std::string ThreadWorker::What()
	{
		return _workerInvokeObj->What();
	}
}