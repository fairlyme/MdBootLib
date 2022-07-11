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
		/// 获取包装的对象
		/// </summary>
		/// <returns></returns>
		virtual IWorker* GetWorker() = 0;

	protected:
		/// <summary>
		/// 被调度
		/// </summary>
		virtual void OnDispatchStarted() = 0;

		/// <summary>
		/// 结束调度
		/// </summary>
		virtual void OnDispatchStoped() = 0;

		/// <summary>
		/// 调度执行前
		/// </summary>
		virtual void OnPreProcess() = 0;

		/// <summary>
		/// 调度执行
		/// </summary>
		virtual void DoProcess() = 0;

		/// <summary>
		/// 完成调度
		/// </summary>
		virtual void OnFinishProcess() = 0;
	};

	/// <summary>
	/// 以线程池方式执行,以单元为任务结构
	/// </summary>
	class IDependent{
	protected:

	public:
		// 常识标识已计划
		virtual bool TryPlan() = 0;
		// 是否已经计划
		virtual bool IsPlaned() = 0;
		// 是否正在运行中
		virtual bool IsWorking() = 0;
		// 启动
		virtual bool Start() = 0;
		// 启动
		virtual bool Stop() = 0;
	};

	/// <summary>
	/// 以独立线程方式执行
	/// </summary>
	class IIndependent {
	public:
		// 是否正在运行中
		virtual bool IsWorking() = 0;
		// 启动 异步
		virtual bool StartAsync() = 0;
		// 启动 同步
		virtual bool Start() = 0;
		// 停止
		virtual bool Stop(bool wait) = 0;
		// join
		virtual void Join() = 0;
	};
}