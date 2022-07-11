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
		// singleton ���ù��캯��
		friend class Singleton<ModuleManagerCenter>;
	private:
		// ���캯��
		ModuleManagerCenter() {};

		// ����ģ��
		std::map<std::string, std::shared_ptr<ICreater<IModule>>> _allModules;



	public:
		template<class T>
		static std::string GetModuleName() {
			return abi::__cxa_demangle(typeid(T).name(), 0, 0, 0);
		}

		// �Ƿ�ӵ��ģ�� : @moduleName
		bool HasModule(std::string moduleName);

		// ע��ģ��
		bool RegistModule(std::string name, std::shared_ptr<ICreater<IModule>> creator);

		// ע��ģ��
		template<class T>
		bool RegistModule(std::string name = "") {
			static_assert(std::is_base_of_v<Module, T>);

			if (name == "") {
				name = GetModuleName<T>();
			}

			// ȥ������ namespace
			{
				std::vector<std::string> ret;
				StringUtil::Split(name, ret, "::");
				
				if (ret.size() > 0) {
					name = ret[ret.size() - 1];
				}
			}

			return RegistModule(name, T::GetCreator());
		}

		// �������ƴ���ģ��,����ģ��Ĳ���
		std::shared_ptr<IModule> CreateModule(std::string moduleName, IModuleParam* moduleParam);

		std::shared_ptr<IModule> CreateModule(configor::json* configJsonObj);


		// ��ȡ����ģ�������
		std::vector<std::string> GetAllModuleNames();
	};
}

