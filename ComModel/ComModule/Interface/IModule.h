#pragma once


#include "../../ComDef/ComDef.h"

namespace MdLib {

	class IModuleParam {
	public:
		virtual std::string ModuleName() = 0;
	};

	class IModule {
	public:
		// ģ������
		virtual std::string ModuleName() = 0;
		// 
		virtual bool InitByModuleParam(IModuleParam* param) = 0;
	};
}