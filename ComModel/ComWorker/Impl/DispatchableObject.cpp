#include "DispatchableObject.h"
namespace MdLib {
    DispatchableObject::DispatchableObject(IWorker* workerInvokeObjRawPtr, WorkerType workerType)
        : _workerType(workerType)
    {
        _iWorkerInvoke = std::shared_ptr<IWorker>(workerInvokeObjRawPtr, [](IWorker* ptr) {});
    }

    DispatchableObject::DispatchableObject(std::shared_ptr<IWorker> wokerInvokeObj, WorkerType workerType)
        : _iWorkerInvoke(wokerInvokeObj), _workerType(workerType)
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

    /// <summary>
    /// 调度执行前
    /// </summary>

    void DispatchableObject::OnPreProcess() {
        _iWorkerInvoke->OnProcessStart();
    }

    /// <summary>
    /// 调度执行
    /// </summary>

    void DispatchableObject::DoProcess() {
        OnPreProcess();
        _iWorkerInvoke->DoProcessOnce();
        OnFinishProcess();
    }

    /// <summary>
    /// 完成调度
    /// </summary>

    void DispatchableObject::OnFinishProcess() {
        _iWorkerInvoke->OnProcessStop();
    }

    void DispatchableObject::OnDispatchStarted()
    {

    }

    // ------ IDispatchableObj ------ 

    WorkerType DispatchableObject::GetType() {
        return _workerType;
    }

}