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
		// singleton ���ù��캯��
		friend class Singleton<ModuleManagerCenter>;
	private:
		// ���캯��
		ModuleManagerCenter() {};

		// ����ģ��
		std::map<std::string, std::function<IModule* (IModuleParam*)>> _allModules;

		// ע��ģ��
		bool RegistModule(std::string name, std::function<IModule* (IModuleParam*)> creator);


	public:
		// �Ƿ�ӵ��ģ�� : @moduleName
		bool HasModule(std::string moduleName);

		// ע��ģ��
		template<class T>
		bool RegistModule() {
			std::string name(abi::__cxa_demangle(typeid(T).name(), 0, 0, 0));

			// ȥ������ namespace
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

		// �������ƴ���ģ��,����ģ��Ĳ���
		IModule* CreateModule(std::string moduleName, IModuleParam* moduleParam);

		// IModule* CreateModule(std::string moduleName, const char* ...);

		// ��ȡ����ģ�������
		std::vector<std::string> GetAllModuleNames();
	};
}

