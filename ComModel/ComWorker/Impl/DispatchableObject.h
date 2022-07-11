#pragma once
// sys headers
#include <vector>
#include <memory>
#include <mutex>

// lib headers 
#include "../ModuleComDef/ComDef.h"
#include "../../../ExternalDep/Singleton/Singleton.h"
#include "../../../ExternalDep/ThreadPool/thread_pool.hpp"
#include "../../../ExternalDep/configor/include/configor/json.hpp"

// module headers
#include "../Interface/IDispatchable.h"

#include "WorkerParamBase.h"
#include "Preset/ModuleWorker.h"
#include "Preset/ThreadWorker.h" // 单独运行

namespace MdLib {
	/// <summary>
	/// dispatch able obj
	/// </summary>
	class DispatchableObject : public IDispatchableObj {
		friend class ModuleDispatcher;

	protected:
		bool _managed = false;
		const WorkerType _workerType;
		std::shared_ptr<IWorker> _iWorkerInvoke;

	protected:
		DispatchableObject(IWorker* workerInvokeObjRawPtr, WorkerType workerType);

	public:
		DispatchableObject(std::shared_ptr<IWorker> wokerInvokeObj, WorkerType workerType);

		~DispatchableObject();


		// ------ IDispatchableObj ------ 
	public:
		/// <summary>
		/// 获取调度类型
		/// </summary>
		/// <returns></returns>
		virtual WorkerType GetType();

		/// <summary>
		/// 获取包装的对象
		/// </summary>
		/// <returns></returns>
		virtual IWorker* GetWorker() override;

	protected:
		/// <summary>
		/// 调度执行前
		/// </summary>
		virtual void OnPreProcess() override;

		/// <summary>
		/// 调度执行
		/// </summary>
		virtual void DoProcess() override;

		/// <summary>
		/// 完成调度
		/// </summary>
		virtual void OnFinishProcess() override;

		virtual void OnDispatchStarted() override;

		virtual void OnDispatchStoped() override {

		}
		// ====== IDispatchableObj ====== 
	};
}
