#pragma once
// sys headers
#include <string>
#include <cxxabi.h>

// lib headers
#include "../../Utils/TypeUtil/TypeCheckUtil.h"
#include "../../ComDef/Errors/CustomErrors.h"

// module headers
#include "../Interface/IModule.h"

namespace MdLib {
	 

	template<class ModuleParamType,class CreaterType>
	class Module : public IModule
	{
		std::string _moduleName;
		
	public:
		Module()
		{
			TypeCheckUtil::AssertIsDelivedType<IModuleParam, ModuleParamType>();
			_moduleName = abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
		}

		Module(IModuleParam* param) : Module(){
			if (InitByModuleParam(param)) {
				throw InvalidParamErr("Invalid module param.");
			}
		}

		Module(configor::json* jsonObj) : Module() {
			if (InitParamByJson(jsonObj)) {
				throw InvalidParamErr("Invalid module param.");
			}
		}

		~Module() {

		}

		// 模块名称
		virtual std::string ModuleName() override {
			return _moduleName;
		}

		// 初始化
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

		virtual bool InitParamByJson(configor::json* jsonObj) {
			ModuleParamType* param = new ModuleParamType();
			if (param->InitParamByJson(jsonObj) && Init(param)) {
				delete param;
				return true;
			}
			delete param;
			return false;
		}

		static std::shared_ptr<ICreater<IModule>> GetCreater() {

		}

		// To Impl
		virtual bool Init(ModuleParamType* param) = 0;

	};
}

