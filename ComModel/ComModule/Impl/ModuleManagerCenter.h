#pragma once
// sys headers
#include <map>
#include <string>
#include <functional>
#include <cxxabi.h>

// lib headers
#include "../../../ExternalDep/Singleton/Singleton.h"
#include "../../Utils/StringUtils/StringUtil.h"

// module headers
#include "../Interface/IModule.h"
#include "Module.h"

namespace MdLib {
	class ModuleManagerCenter : public Singleton<ModuleManagerCenter>
	{
		// singleton 调用构造函数
		friend class Singleton<ModuleManagerCenter>;
	private:
		// 构造函数
		ModuleManagerCenter() {};

		// 所有模块
		std::map<std::string, std::function<IModule* (IModuleParam*)>> _allModules;

		// 注册模块
		bool RegistModule(std::string name, std::function<IModule* (IModuleParam*)> creator);


	public:
		// 是否拥有模块 : @moduleName
		bool HasModule(std::string moduleName);

		// 注册模块
		template<class T>
		bool RegistModule() {
			std::string name(abi::__cxa_demangle(typeid(T).name(), 0, 0, 0));

			// 去掉所有 namespace
			{
				std::vector<std::string> ret;
				StringUtil::Split(name, ret, "::");
				
				if (ret.size() > 0) {
					name = ret[ret.size() - 1];
				}
			}

			std::function<IModule* (IModuleParam*)> creator = [](IModuleParam* mdParam) -> IModule* {
				T* ret = new T();
				if (ret->InitByModuleParam(mdParam))
				{
					return ret;
				}
				else
				{
					delete ret;
					return nullptr;
				}
			};

			return RegistModule(name, creator);
		}

		// 根据名称创建模块,传入模块的参数
		IModule* CreateModule(std::string moduleName, IModuleParam* moduleParam);

		// IModule* CreateModule(std::string moduleName, const char* ...);

		// 获取所有模块的名称
		std::vector<std::string> GetAllModuleNames();
	};
}

