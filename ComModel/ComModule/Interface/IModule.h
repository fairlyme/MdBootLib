#pragma once
// sys header
#include <memory>

// lib header
#include "../../../ExternalDep/configor/include/configor/json.hpp"

// module header
#include "../../ComDef/ComDef.h"

namespace MdLib {
	class IModuleParam {
	public:
		virtual std::string ModuleName() = 0;

		virtual bool InitParamByJson(configor::json* jobj) = 0;
	};

	template<class T>
	class ICreater {
	public:
		virtual std::shared_ptr<T> Create(IModuleParam* param) = 0;
		virtual std::shared_ptr<T> Create(configor::json* jobj) = 0;
	};

	class IModule {
	public:
		
		/// <summary>
		/// 获取模块名称
		/// </summary>
		/// <returns></returns>
		virtual std::string ModuleName() = 0;
		
		/// <summary>
		/// 通过参数指针进行初始化
		/// </summary>
		/// <param name="param"></param>
		/// <returns></returns>
		virtual bool InitByModuleParam(IModuleParam* param) = 0;

		/// <summary>
		/// 通过json参数进行初始化
		/// </summary>
		/// <param name="jobj"></param>
		/// <returns></returns>
		virtual  bool InitParamByJson(configor::json* jobj) = 0;

		/// <summary>
		/// 拥有新工作需要执行
		/// </summary>
		/// <returns></returns>
		virtual bool HasWork() = 0;

		/// <summary>
		/// 执行工作
		/// </summary>
		virtual void DoWork() = 0;

		/// <summary>
		/// 模块结束生命
		/// </summary>
		virtual void OnStop() = 0;

		/// <summary>
		/// 模块开始运作
		/// </summary>
		virtual void OnStart() = 0;

		/// <summary>
		/// 工作是否已经完全结束.
		/// </summary>
		virtual bool IsWorkDone() = 0;
	};
}