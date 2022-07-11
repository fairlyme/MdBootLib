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
		/// ��ȡģ������
		/// </summary>
		/// <returns></returns>
		virtual std::string ModuleName() = 0;
		
		/// <summary>
		/// ͨ������ָ����г�ʼ��
		/// </summary>
		/// <param name="param"></param>
		/// <returns></returns>
		virtual bool InitByModuleParam(IModuleParam* param) = 0;

		/// <summary>
		/// ͨ��json�������г�ʼ��
		/// </summary>
		/// <param name="jobj"></param>
		/// <returns></returns>
		virtual  bool InitParamByJson(configor::json* jobj) = 0;

		/// <summary>
		/// ӵ���¹�����Ҫִ��
		/// </summary>
		/// <returns></returns>
		virtual bool HasWork() = 0;

		/// <summary>
		/// ִ�й���
		/// </summary>
		virtual void DoWork() = 0;

		/// <summary>
		/// ģ���������
		/// </summary>
		virtual void OnStop() = 0;

		/// <summary>
		/// ģ�鿪ʼ����
		/// </summary>
		virtual void OnStart() = 0;

		/// <summary>
		/// �����Ƿ��Ѿ���ȫ����.
		/// </summary>
		virtual bool IsWorkDone() = 0;
	};
}