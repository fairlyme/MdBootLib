#pragma once
// sys headers

// module headers 
#include "../ModuleComDef/ComDef.h"
#include "../Interface/IWorkerParam.h"

namespace MdLib {
	class WorkerParamBase : public IWorkerParam
	{
	private:
		std::string _workerKey;
		WorkerType _wokerType;

	public:
		WorkerParamBase(std::string workerKey,WorkerType workerType) : _workerKey(workerKey) , _wokerType(workerType){}
		// Worker key
		virtual std::string WorkerKey() override {
			return _workerKey;
		}

		// worker type
		virtual WorkerType GetWorkerType() override {
			return _wokerType;
		}
	};
}

