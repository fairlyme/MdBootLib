#pragma once
// sys header
#include <mutex>

// lib header

// module header
#include "../ModuleComDef/ComDef.h"
#include "DispatchableObject.h"


namespace MdLib {

	/// <summary>
	/// �Ƕ����߳� worker
	///		�Ƕ����̵߳���
	///		start -> ѯ���Ƿ��й��� -> Plan -> Do
	/// </summary>
	class DependentDispatchObject : public DispatchableObject, public IDependent {
	private:
		std::mutex _stateMtx;
		bool _isPlaned = false;
		bool _runFlag = false;
		bool _isRunning = false;

	protected:
		DependentDispatchObject(IWorker* workerInvokeObjRawPtr);

	public:
		DependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj);

		virtual bool TryPlan() override;
		// �Ƿ��Ѿ��ƻ�
		virtual bool IsPlaned() override;
		// �Ƿ�����������
		virtual bool IsWorking() override;

		// ����
		virtual bool Start() override {
			return true;
		}
		// ����
		virtual bool Stop() override {
			return true;
		}

	protected:
		/// <summary>
		/// IDispatchableObj::OnPreProcess
		///		����״̬�޸� 
		/// </summary>
		virtual void OnPreProcess() override;

		/// <summary>
		/// IDispatchableObj::OnFinishProcess
		///		����״̬�޸� 
		/// </summary>
		virtual void OnFinishProcess() override;
	};
}
