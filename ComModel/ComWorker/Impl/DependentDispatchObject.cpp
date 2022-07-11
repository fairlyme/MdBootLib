#include "DependentDispatchObject.h"
namespace MdLib {

    DependentDispatchObject::DependentDispatchObject(IWorker* workerInvokeObjRawPtr)
        : DispatchableObject(workerInvokeObjRawPtr, WorkerType::E_Dependent)
    {
    }

    DependentDispatchObject::DependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj)
        : DispatchableObject(wokerInvokeObj, WorkerType::E_Dependent) {

    }

    bool DependentDispatchObject::TryPlan() {
        if (IsPlaned()) {
            return false;
        }
        else
        {
            std::lock_guard<std::mutex> guard(_stateMtx);
            if (IsPlaned()) {
                _isPlaned = true;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    // 是否已经计划

    bool DependentDispatchObject::IsPlaned() { return _isPlaned; }

    // 是否正在运行中

    bool DependentDispatchObject::IsWorking() { return _isRunning; }

    void DependentDispatchObject::OnPreProcess() {
        DispatchableObject::OnPreProcess();

        std::lock_guard guard(_stateMtx);
        _isPlaned = false;
        _isRunning = true;
        _runFlag = true;
    }

    void DependentDispatchObject::OnFinishProcess() {
        DispatchableObject::OnFinishProcess();

        std::lock_guard guard(_stateMtx);
        _isRunning = false;
    }
}