#include "IndependentDispatchObject.h"

namespace MdLib {
    IndependentDispatchObject::IndependentDispatchObject(IWorker* workerInvokeObjRawPtr)
        : DispatchableObject(workerInvokeObjRawPtr, WorkerType::E_Independent) {
    }

    void IndependentDispatchObject::_DoProcessWarp() {
        {
            std::lock_guard guard(_stateMtx);
            _isRunning = true;
        }

        _iWorkerInvoke->OnProcessStart();
        while (_runningFlag)
        {
            if (_iWorkerInvoke->CanProcess()) {
                _iWorkerInvoke->DoProcessOnce();
            }
            else
            {
                std::this_thread::yield();
            }

            if (_iWorkerInvoke->IsProcessAllDone()) {
                std::lock_guard guard(_stateMtx);
                _runningFlag = false;
            }
        }
        _iWorkerInvoke->OnProcessStop();

    ALL_FINISH:
        {
            std::lock_guard guard(_stateMtx);
            _isRunning = false;
        }
    }

    IndependentDispatchObject::IndependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj)
        : DispatchableObject(wokerInvokeObj, WorkerType::E_Independent) {

    }

    bool IndependentDispatchObject::IsWorking() {
        return _isRunning;
    }

    // 启动 异步
    bool IndependentDispatchObject::StartAsync() {
        if (_isRunning && GetWorker() == nullptr) {
            return false;
        }

        {
            std::lock_guard guard(_stateMtx);
            _isRunning = true;
            _runningFlag = true;
        }

        _workThd = std::thread(&IndependentDispatchObject::_DoProcessWarp, this);

        pthread_setname_np(_workThd.native_handle(), GetWorker()->What().c_str());

        return true;
    }

    bool IndependentDispatchObject::Start() {
        if (_isRunning && GetWorker() == nullptr) {
            return false;
        }
        {
            _isRunning = true;
            _runningFlag = true;
        }

        _DoProcessWarp();

        return true;
    }

    // 停止
    bool IndependentDispatchObject::Stop(bool wait) {
        if (!_isRunning) {
            return true;
        }

        {
            std::lock_guard guard(_stateMtx);
            _runningFlag = false;
        }

        if (wait) {
            _workThd.join();
        }

        return !_isRunning;
    }
    
    void IndependentDispatchObject::Join()
    {
        if (!_isRunning) {
            return;
        }

        if (_workThd.joinable()) {
            _workThd.join();
        }
        else
        {
            // 错误状态,线程并没有在运行
            std::lock_guard guard(_stateMtx);
            _isRunning = false;
        }
    }
}