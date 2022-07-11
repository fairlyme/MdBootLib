#pragma once
// sys headers
#include <map>
#include <string>
#include <functional>
#include <cxxabi.h>
#include <type_traits>

// lib headers
#include "../../../ExternalDep/Singleton/Singleton.h"
#include "../../Utils/StringUtils/StringUtil.h"
#include "../../../ExternalDep/configor/include/configor/json.hpp"

// module headers
#include "../Interface/IModule.h"

#include "../../ComDataDistribute/ComDataExchange.h"

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
		std::map<std::string, std::shared_ptr<ICreater<IModule>>> _allModules;



	public:
		template<class T>
		static std::string GetModuleName() {
			return abi::__cxa_demangle(typeid(T).name(), 0, 0, 0);
		}

		// 是否拥有模块 : @moduleName
		bool HasModule(std::string moduleName);

		// 注册模块
		bool RegistModule(std::string name, std::shared_ptr<ICreater<IModule>> creator);

		// 注册模块
		template<class T>
		bool RegistModule(std::string name = "") {
			static_assert(std::is_base_of_v<Module, T>);

			if (name == "") {
				name = GetModuleName<T>();
			}

			// 去掉所有 namespace
			{
				std::vector<std::string> ret;
				StringUtil::Split(name, ret, "::");
				
				if (ret.size() > 0) {
					name = ret[ret.size() - 1];
				}
			}

			return RegistModule(name, T::GetCreator());
		}

		// 根据名称创建模块,传入模块的参数
		std::shared_ptr<IModule> CreateModule(std::string moduleName, IModuleParam* moduleParam);

		std::shared_ptr<IModule> CreateModule(configor::json* configJsonObj);


		// 获取所有模块的名称
		std::vector<std::string> GetAllModuleNames();
	};
}

