#pragma once
// sys headers
#include <string>

// module headers 
#include "../ModuleComDef/ComDef.h"

namespace MdLib {
	class IWorkerParam {
	public:
		// Worker key
		virtual std::string WorkerKey() = 0;

		// worker type
		virtual WorkerType GetWorkerType() = 0;
	};
}