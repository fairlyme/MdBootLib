#include "ModuleDispatcher.h"
#include "../../ComModule/ComModule.h"
#include "Preset/ModuleWorker.h"
#include <future>

namespace MdLib {
    ModuleDispatcher::ModuleDispatcher()
    {
    }


    bool ModuleDispatcher::Init(int maxIndepThreadCnt, int maxDepThdCnt, int maxDepWorkerCnt) {
        if (_threadPool == nullptr) {
            // 本身占据一个线程
            _threadPool = new thread_pool(maxDepThdCnt);
            return true;
        }
        else
        {
            return false;
        }
    }


    bool ModuleDispatcher::StopAll()
    {
        _allFinish = true;

        _allFinishCv.notify_all();
        return true;
    }

    void ModuleDispatcher::WaitAllTask()
    {
        if (!_allFinish) {
            std::mutex waitMtx;
            std::unique_lock<std::mutex> lk(waitMtx);

            _allFinishCv.wait(lk);
        }
    }

    DispatchableObject::DispatchableObject(IWorker* workerInvokeObjRawPtr)
    {
        _iWorkerInvoke = std::shared_ptr<IWorker>(workerInvokeObjRawPtr, [](IWorker* ptr) {});
    }

    DispatchableObject::DispatchableObject(std::shared_ptr<IWorker> wokerInvokeObj)
        :_iWorkerInvoke(wokerInvokeObj)
    {

    }

    DispatchableObject::~DispatchableObject() {

    }

    /// <summary>
    /// 获取包装的对象
    /// </summary>
    /// <returns></returns>
    IWorker* DispatchableObject::GetWorker() {
        return _iWorkerInvoke.get();
    }

    DependentDispatchObject::DependentDispatchObject(IWorker* workerInvokeObjRawPtr)
        : DispatchableObject(workerInvokeObjRawPtr)
    {
    }

    DependentDispatchObject::DependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj) : DispatchableObject(wokerInvokeObj) {

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

    IndependentDispatchObject::IndependentDispatchObject(IWorker* workerInvokeObjRawPtr) : DispatchableObject(workerInvokeObjRawPtr) {
    }

    IndependentDispatchObject::IndependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj) : DispatchableObject(wokerInvokeObj) {

    }
}