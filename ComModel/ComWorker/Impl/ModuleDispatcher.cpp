#include "ModuleDispatcher.h"
#include "../../ComModule/ComModule.h"
#include "Preset/ModuleWorker.h"
#include <future>

namespace MdLib {
    ModuleDispatcher::ModuleDispatcher()
    {
    }

    void ModuleDispatcher::OnProcessStart()
    {
    }

    void ModuleDispatcher::DoProcessOnce()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        printf("ModuleDispatcher working \n");
    }

    void ModuleDispatcher::OnProcessStop()
    {
    }


    std::string ModuleDispatcher::What()
    {
        return std::string();
    }

    bool ModuleDispatcher::Init(int maxIndepThreadCnt, int maxDepThdCnt, int maxDepWorkerCnt) {
        if (_threadPool == nullptr) {
            // ����ռ��һ���߳�
            _threadPool = new thread_pool(maxDepThdCnt);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ModuleDispatcher::StartWorker(std::string moduleName, WorkerType dispatchType, IModuleParam* mdParam)
    {
        // ����module
        auto md = ModuleManagerCenter::getInstance().CreateModule(moduleName, mdParam);

        if (md == nullptr) {
            return false;
        }

        // ����ModuleWorker
        ModuleWorker* worker = new ModuleWorker(std::shared_ptr<IModule>(md));


        return DispatchWorker(std::shared_ptr<IWorker>((IWorker*)worker), dispatchType);
    }

    bool ModuleDispatcher::DispatchWorker(std::shared_ptr<IWorker> worker, WorkerType dispatchType)
    {
        if (dispatchType == WorkerType::E_Dependent) {
            
        }
    }

    bool ModuleDispatcher::StopAll()
    {
        _allFinish = true;
        _allFinishCv.notify_all();
        StopProcess();
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
    /// ��ȡ��װ�Ķ���
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

    // �Ƿ��Ѿ��ƻ�

    bool DependentDispatchObject::IsPlaned() { return _isPlaned; }

    // �Ƿ�����������

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