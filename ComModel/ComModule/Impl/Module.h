#pragma once
// sys headers
#include <string>
#include <cxxabi.h>

// lib headers
#include "../../Utils/TypeUtil/TypeCheckUtil.h"

// module headers
#include "../Interface/IModule.h"

namespace MdLib {
	class ModuleParam : public IModuleParam{

	};


	template<class ModuleParamType>
	class Module : public IModule
	{
		std::string _moduleName;
		
	public:
		Module()
		{
			TypeCheckUtil::AssertIsDelivedType<IModuleParam, ModuleParamType>();
			_moduleName = abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
		}

		~Module() {

		}

		virtual bool Init(ModuleParamType* param) = 0;

		// Ä£¿éÃû³Æ
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

