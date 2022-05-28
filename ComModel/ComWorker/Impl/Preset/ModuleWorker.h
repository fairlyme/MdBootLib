#pragma once
#include <memory>
#include "../../Interface/IWorker.h"
#include "../../../ComModule/Impl/Module.h"

namespace MdLib {
	/*
	* ģ������
	*/
	class ModuleWorker : public IWorker
	{
		std::shared_ptr<IModule> _mdPtr;
	public:
		ModuleWorker(std::shared_ptr<IModule> mdPtr);

		// ͨ�� VWorker �̳�
		virtual void DoProcessOnce() override;
		virtual void OnProcessStop() override;
		virtual void OnProcessStart() override;
		virtual std::string What() override;
	};
}

