#pragma once
// module headers
#include "../Interface/IModule.h"
#include <any>
#include <vector>
#include <type_traits>
#include <typeinfo>

namespace MdLib {
	/// <summary>
	/// ²ÎÊýÏî
	/// </summary>
	struct _ParamInfo {
	public:
		std::string Name;
		size_t TypeHash;
		std::any Val;
		bool Required;

		_ParamInfo(std::string name, size_t typeHash, std::any val, bool required)
			: Name(name), TypeHash(typeHash), Val(val), Required(required)
		{

		}
	};

	template <class _Ty, class... _Types>
	constexpr bool _Is_any_of_v = // true if and only if _Ty is in _Types
		std::disjunction_v<std::is_same<_Ty, _Types>...>;

	class ModuleParamBase : public IModuleParam
	{
	protected:
		std::vector<_ParamInfo> _params;

		template<typename T, std::enable_if_t<_Is_any_of_v<T, float, std::string, int>, bool> PH = true>
		void RequiredVal(std::string name,bool required,T* paramRef) 
		{
			_params.emplace_back(_ParamInfo(name,typeid(T).hash_code(), std::any(paramRef), required));
		}

		bool GetRequiredParam(configor::json& jobj, _ParamInfo& paramVal) {
			auto typeVal = paramVal.Val;
			
			if (typeid(int).hash_code() == paramVal.TypeHash) {
				// int
				int* tarAddr = std::any_cast<int*>(typeVal);

				if (jobj[paramVal.Name].get<int>(*tarAddr)) {
					return true;
				}
				else
				{
					if (paramVal.Required) {
						goto THROW_REQ_ERROR;
					}
					else
					{
						return false;
					}
				}
			}
			else if(typeid(std::string).hash_code() == paramVal.TypeHash)
			{				
				// std::string
				std::string * tarAddr = std::any_cast<std::string*>(typeVal);

				if (jobj[paramVal.Name].get<std::string>(*tarAddr)) {
					return true;
				}
				else
				{
					if (paramVal.Required) {
						goto THROW_REQ_ERROR;
					}
					else
					{
						return false;
					}
				}
			}
			else if (typeid(float).hash_code() == paramVal.TypeHash)
			{
				// std::string
				float* tarAddr = std::any_cast<float*>(typeVal);

				if (jobj[paramVal.Name].get<float>(*tarAddr)) {
					return true;
				}
				else
				{
					if (paramVal.Required) {
						goto THROW_REQ_ERROR;
					}
					else
					{
						return false;
					}
				}
			}
			else {
				return false;
			}

		THROW_REQ_ERROR:
			assert(false);
		}

	public:

		virtual bool InitParamByJson(configor::json* jobj) override {
			for (auto p : _params) {
				GetRequiredParam(*jobj, p);
			}

			return true;
		}
	};
}

