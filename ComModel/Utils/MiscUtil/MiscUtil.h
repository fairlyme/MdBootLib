#pragma once
#include <string>

#include <cxxabi.h>
namespace MdLib {
	class MiscUtil
	{
	public:
		template<typename T>
		static std::string GetTypeName();

		template<typename T>
		static std::string GetTypeName(T t);

		template<typename T>
		static std::string GetPointerTypeName(T* t);
	};

	template<typename T>
	std::string MiscUtil::GetTypeName()
	{
		return std::string(abi::__cxa_demangle(typeid(T).name(), 0, 0, 0));
	}

	template<typename T>
	std::string MiscUtil::GetTypeName(T t)
	{
		return std::string(abi::__cxa_demangle(typeid(t).name(), 0, 0, 0));
	}

	template<typename T>
	std::string MiscUtil::GetPointerTypeName(T* t)
	{
		return std::string(abi::__cxa_demangle(typeid(*t).name(), 0, 0, 0));
	}
}
