#pragma once
#include <memory>
#include "../../Interface/IWorker.h"
#include "../../../ComModule/Impl/Module.h"

namespace MdLib {
	/*
	* 模块运行
	*/
	class ModuleWorker : public IWorker
	{
		std::shared_ptr<IModule> _mdPtr;
	public:
		ModuleWorker(std::shared_ptr<IModule> mdPtr);

		// 通过 VWorker 继承
		virtual void DoProcessOnce() override;
		virtual void OnProcessStop() override;
		virtual void OnProcessStart() override;
		virtual std::string What() override;
	};
}

