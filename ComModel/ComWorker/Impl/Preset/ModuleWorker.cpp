#include "ModuleWorker.h"

#include "../../../Utils/MiscUtil/MiscUtil.h"

#include <ExternalDep/plog/Log.h>

namespace MdLib{
    ModuleWorker::ModuleWorker(std::shared_ptr<IModule> mdPtr) {
        _mdPtr = mdPtr;
    }

    void ModuleWorker::DoProcessOnce()
    {
        _mdPtr->DoWork();
    }

    void ModuleWorker::OnProcessStop()
    {
        _mdPtr->OnStop();
    }

    void ModuleWorker::OnProcessStart()
    {
        _mdPtr->OnStart();
    }

    bool ModuleWorker::IsProcessAllDone()
    {
        return _mdPtr->IsWorkDone();
    }

    bool ModuleWorker::CanProcess() {
        return _mdPtr->HasWork();
    }

    std::string ModuleWorker::What()
    {
        return std::string("ModuleWorker|") + MiscUtil::GetPointerTypeName(_mdPtr.get());
    }
}