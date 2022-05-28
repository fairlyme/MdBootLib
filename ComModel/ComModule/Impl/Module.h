#pragma once
// sys headers
#include <string>
#include <cxxabi.h>

// lib headers


// module headers
#include "../Interface/IModule.h"

namespace MdLib {
	class IModuleParam {
	public:
		virtual std::string ModuleName() = 0;
	};

	class IModule {
	public:
		// 模块名称
		virtual std::string ModuleName() = 0;
		// 
		virtual bool InitByModuleParam(IModuleParam* param) = 0;
	};

	template<class ModuleParamType>
	class Module : public IModule
	{
		std::string _moduleName;
		
	public:
		Module()
		{
			_moduleName = abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
		}

		~Module() {

		}

		virtual bool Init(ModuleParamType* param) = 0;

		// 模块名称
		virtual std::string ModuleName() override {
			return _moduleName;
		}

		virtual bool InitByModuleParam(IModuleParam* param) override {
			if (param == nullptr) {
				return Init(nullptr);
			}

			ModuleParamType* actualParam = dynamic_cast<ModuleParamType*>(param);

			if (actualParam == nullptr) {
				return false;
			}

			return Init(actualParam);
		}
	};
}

