#pragma once
// sys headers

// module headers 
#include "../ModuleComDef/ComDef.h"

namespace MdLib {
	class IWorker {
	public:
		/// <summary>
		/// 实现 处理逻辑, 不需要管运行状态
		/// 处理一次, 尽量不要持续太长时间
		/// </summary>
		virtual void DoProcessOnce() = 0;

		/// <summary>
		/// 处理退出
		/// </summary>
		virtual void OnProcessStop() = 0;

		/// <summary>
		/// 处理开始
		/// </summary>
		virtual void OnProcessStart() = 0;

		/// <summary>
		/// work 名称
		/// </summary>
		/// <returns></returns>
		virtual std::string What() = 0;

	};
}