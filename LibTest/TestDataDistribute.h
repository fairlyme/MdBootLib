#pragma once

#include "ComModel/ComDef/ComDef.h"
#include "ComModel/ComDataDistribute/ComDataExchange.h"

class TestData {
protected:
	std::string _what;
public:
	TestData(std::string what)
		: _what(what)
	{

	}

	std::string What() {
		return _what;
	}
};

class TestProcuder : public MdLib::DistributeProductor<TestData> {
private:
	int _preID = -1;


public:
	TestProcuder() :MdLib::DistributeProductor<TestData>("TestProcuder", "TestData") {}

	void ProduceOne() {
		if (!HasContainer()) {
			return;
		}

		char name[25] = { 0 };
		sprintf(name, "item %d", ++_preID);

		auto product = std::make_shared<TestData>(name);

		if (GetDistributeContainer()->Put(product)) {
			printf("product %s sucess \n", product->What().c_str());
		}
		else
		{
			printf("product %s failed \n", product->What().c_str());
		}
	}
};


class TestConsumer : public MdLib::DistributeConsumer<TestData> {
private:

public:
	TestConsumer() 
		: MdLib::DistributeConsumer<TestData>("DistributeConsumer", "TestData") {
		
	}
	virtual bool Consume(TestData* product) override {
		printf("sucess take %s \n", product->What().c_str());
		return true;
	}

	virtual bool IsConsumeDone() {
		return false;
	}
};

void TestDataDistribution();