#pragma once
// sys header
#include <mutex>

// lib header

// module header
#include "../ModuleComDef/ComDef.h"
#include "DispatchableObject.h"


namespace MdLib {

	/// <summary>
	/// 非独立线程 worker
	///		非独立线程调度
	///		start -> 询问是否有工作 -> Plan -> Do
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
		// 是否已经计划
		virtual bool IsPlaned() override;
		// 是否正在运行中
		virtual bool IsWorking() override;

		// 启动
		virtual bool Start() override {
			return true;
		}
		// 启动
		virtual bool Stop() override {
			return true;
		}

	protected:
		/// <summary>
		/// IDispatchableObj::OnPreProcess
		///		增加状态修改 
		/// </summary>
		virtual void OnPreProcess() override;

		/// <summary>
		/// IDispatchableObj::OnFinishProcess
		///		增加状态修改 
		/// </summary>
		virtual void OnFinishProcess() override;
	};
}
