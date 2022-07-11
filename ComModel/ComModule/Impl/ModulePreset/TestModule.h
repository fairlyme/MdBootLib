#pragma once
#include "../Module.h"

namespace MdLib {
	class TestModuleParam
		: public IModuleParam
	{
	public:

		virtual std::string ModuleName() {
			return "TestModule";
		}


		virtual bool InitParamByJson(configor::json* jobj) {
			return true;
		}
	};

	class TestModule : public Module<TestModuleParam>{
	public:
		TestModule() : Module<TestModuleParam>() {}
		virtual bool Init(TestModuleParam* param) override {
			return true;
		}
	};
}