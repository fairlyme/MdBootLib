#include "ModuleDispatcher.h"
#include "../../ComModule/ComModule.h"
#include "Preset/ModuleWorker.h"
#include <future>

#include <ExternalDep/plog/Log.h>

namespace MdLib {
    ModuleDispatcher::ModuleDispatcher()
    {
    }

    void ModuleDispatcher::OnProcessStart()
    {
    }

    bool ModuleDispatcher::CanProcess() {
        return true;
    }

    bool ModuleDispatcher::IsProcessAllDone() {
        return false;
    }

    bool ModuleDispatcher::_ApplyDependentWorker(std::shared_ptr<DependentDispatchObject> worker) {
        if (_allDependentWorker.size() >= _maxDependentWorkerCnt) {
            return false;
        }

        _allDependentWorker.emplace_back(worker);

        return true;
    }

    bool ModuleDispatcher::_ApplyIndependetWorker(std::shared_ptr<IndependentDispatchObject> worker) {
        if (worker->StartAsync()) {
            _allInDependentWorker.emplace_back(worker);
            return true;
        }
        return false;
    }

    void ModuleDispatcher::DoProcessOnce()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        printf("current dependent worker count : %d \n", _allDependentWorker.size());
        printf("current independent worker count : %d \n", _allInDependentWorker.size());
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
            // 本身占据一个线程
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
        // 创建module
        auto md = ModuleManagerCenter::getInstance().CreateModule(moduleName, mdParam);

        if (md == nullptr) {
            return false;
        }

        // 构造ModuleWorker
        ModuleWorker* worker = new ModuleWorker(std::shared_ptr<IModule>(md));


        return DispatchWorker(std::shared_ptr<IWorker>((IWorker*)worker), dispatchType);
    }

    bool ModuleDispatcher::StartWorker(configor::json& configJsonObj)
    {
        // 获取所有参数 dispatch type,
        WorkerType workType = WorkerType::E_UNKNOWN;
        configor::json jsonObj = configJsonObj["RunType"];
        if (jsonObj.is_string()) {
            std::string typeOriStr = jsonObj.as_string();
            std::string typeStr = StringUtil::ToLower(typeOriStr);
            if (typeStr == "dependent") {
                workType = WorkerType::E_Dependent;
            }
            else if (typeStr == "independent") {
                workType = WorkerType::E_Independent;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }


        std::shared_ptr<IModule> md = ModuleManagerCenter::getInstance().CreateModule(&configJsonObj);

        if (md == nullptr) {
            return false;
        }

        // 构造ModuleWorker
        ModuleWorker* worker = new ModuleWorker(std::shared_ptr<IModule>(md));

        return DispatchWorker(std::shared_ptr<IWorker>((IWorker*)worker), workType);
    }

    bool ModuleDispatcher::DispatchWorker(std::shared_ptr<IWorker> worker, WorkerType dispatchType)
    {
        if (dispatchType == WorkerType::E_Dependent) {
            return _ApplyDependentWorker(std::make_shared<DependentDispatchObject>(worker));
        }
        else if (dispatchType == WorkerType::E_Independent) {
            return _ApplyIndependetWorker(std::make_shared<IndependentDispatchObject>(worker));
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

        for (auto indepItem : _allInDependentWorker) {
            indepItem->Stop(false);
        }

        for (auto indepItem : _allInDependentWorker) {
            indepItem->Join();
        }

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
}