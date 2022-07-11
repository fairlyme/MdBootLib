#pragma once
#include "DispatchableObject.h"

namespace MdLib {
	/// <summary>
	/// �Ƕ����߳� worker
	///		����
	///		StartAsync -> Stop
	/// </summary>
	class IndependentDispatchObject : public DispatchableObject, public IIndependent {
	protected:
		std::mutex _stateMtx;
		bool _isRunning = false;
		bool _runningFlag = false;

		std::thread _workThd;
		IndependentDispatchObject(IWorker* workerInvokeObjRawPtr);

		void _DoProcessWarp();

	public:
		// ��������obj
		IndependentDispatchObject(std::shared_ptr<IWorker> wokerInvokeObj);
		// ���ڹ�����
		virtual bool IsWorking() override;
		// ���� �첽
		virtual bool StartAsync() override;
		// ���� ͬ��
		virtual bool Start() override;
		// ֹͣ
		virtual bool Stop(bool wait) override;
		// �ȴ��߳̽���
		virtual void Join() override;
	};

}
