#pragma once
// sys headers

// module headers 
#include "../ModuleComDef/ComDef.h"
#include "IWorker.h"


namespace MdLib {
	/// <summary>
	/// DispatchableObject
	/// </summary>
	class IDispatchableObj {
	public:
		/// <summary>
		/// worker type
		/// </summary>
		/// <returns></returns>
		virtual WorkerType GetType() = 0;

		/// <summary>
		/// ��ȡ��װ�Ķ���
		/// </summary>
		/// <returns></returns>
		virtual IWorker* GetWorker() = 0;

	protected:
		/// <summary>
		/// ������
		/// </summary>
		virtual void OnDispatchStarted() = 0;

		/// <summary>
		/// ��������
		/// </summary>
		virtual void OnDispatchStoped() = 0;

		/// <summary>
		/// ����ִ��ǰ
		/// </summary>
		virtual void OnPreProcess() = 0;

		/// <summary>
		/// ����ִ��
		/// </summary>
		virtual void DoProcess() = 0;

		/// <summary>
		/// ��ɵ���
		/// </summary>
		virtual void OnFinishProcess() = 0;
	};

	/// <summary>
	/// ���̳߳ط�ʽִ��,�Ե�ԪΪ����ṹ
	/// </summary>
	class IDependent{
	protected:

	public:
		// ��ʶ��ʶ�Ѽƻ�
		virtual bool TryPlan() = 0;
		// �Ƿ��Ѿ��ƻ�
		virtual bool IsPlaned() = 0;
		// �Ƿ�����������
		virtual bool IsWorking() = 0;
		// ����
		virtual bool Start() = 0;
		// ����
		virtual bool Stop() = 0;
	};

	/// <summary>
	/// �Զ����̷߳�ʽִ��
	/// </summary>
	class IIndependent {
	public:
		// �Ƿ�����������
		virtual bool IsWorking() = 0;
		// ���� �첽
		virtual bool StartAsync() = 0;
		// ���� ͬ��
		virtual bool Start() = 0;
		// ֹͣ
		virtual bool Stop(bool wait) = 0;
		// join
		virtual void Join() = 0;
	};
}