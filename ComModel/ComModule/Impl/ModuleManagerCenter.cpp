
#include <type_traits>
#include "ModuleManagerCenter.h"
#include "ExternalDep/plog/Log.h"


namespace MdLib {

	bool ModuleManagerCenter::HasModule(std::string moduleName) {
		return _allModules.count(moduleName) > 0;
	}

	bool ModuleManagerCenter::RegistModule(std::string name, std::shared_ptr<ICreater<IModule>> creator) {
		if (HasModule(name)) {
			return false;
		}
		else
		{
			_allModules[name] = creator;
			
			return true;
		}
	}

	std::shared_ptr<IModule> ModuleManagerCenter::CreateModule(std::string moduleName, IModuleParam* moduleParam) {
		if (HasModule(moduleName)) {
			return _allModules[moduleName]->Create(moduleParam);
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<IModule> ModuleManagerCenter::CreateModule(configor::json* configJsonObj)
	{
		configor::json mdNameJsonObj = (*configJsonObj)["ModuleName"];
		if (mdNameJsonObj.is_string()) {
			std::string name = mdNameJsonObj.as_string();
			PLOGI << "create module " << name;
			if (HasModule(name)) {
				return _allModules[name]->Create(configJsonObj);
			}
			else
			{
				PLOGE << "no module named " << name;
				return nullptr;
			}
		}

		PLOGE << "module param error";
		return nullptr;
	}

	std::vector<std::string> ModuleManagerCenter::GetAllModuleNames() {
		std::vector<std::string> ret;
		for (auto it = _allModules.begin(); it != _allModules.end(); it++) {
			ret.emplace_back(it->first);
		}
		return ret;
	}
}