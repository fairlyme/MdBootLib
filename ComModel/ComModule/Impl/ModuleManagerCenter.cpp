
#include <type_traits>
#include "ModuleManagerCenter.h"

namespace MdLib {

	bool ModuleManagerCenter::HasModule(std::string moduleName) {
		return _allModules.count(moduleName) > 0;
	}

	bool ModuleManagerCenter::RegistModule(std::string name, std::function<IModule* (IModuleParam*)> creator) {
		if (HasModule(name)) {
			return false;
		}
		else
		{
			_allModules[name] = creator;
			return true;
		}
	}

	IModule* ModuleManagerCenter::CreateModule(std::string moduleName, IModuleParam* moduleParam) {
		if (HasModule(moduleName)) {
			return _allModules[moduleName](moduleParam);
		}
		else
		{
			return nullptr;
		}
	}

	std::vector<std::string> ModuleManagerCenter::GetAllModuleNames() {
		std::vector<std::string> ret;
		for (auto it = _allModules.begin(); it != _allModules.end(); it++) {
			ret.emplace_back(it->first);
		}
		return ret;
	}
}