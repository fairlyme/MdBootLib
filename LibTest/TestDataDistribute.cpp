#include "TestDataDistribute.h"

#include "ComModel/ComDataDistribute/ComDataExchange.h"
#include <assert.h>

void TestDataDistribution() {
    // 获取 分发中心 实例 
    MdLib::DistributionCenter& center = MdLib::DistributionCenter::getInstance();

    // 构建生产者
    TestProcuder *producer = new TestProcuder();
    std::shared_ptr<MdLib::IDistributeProducer<TestData>> producersPtr 
        = std::shared_ptr<MdLib::IDistributeProducer<TestData>>((MdLib::IDistributeProducer<TestData>*)producer);

    // 注册生产着
    assert(center.RegistDistributeProducer<TestData>(producersPtr));
    assert(!center.RegistDistributeProducer<TestData>(producersPtr));

    // 构建消费者
    TestConsumer* consumer = new TestConsumer();
    std::shared_ptr<MdLib::IDistributeConsumer<TestData>> consumerPtr
        = std::shared_ptr<MdLib::IDistributeConsumer<TestData>>((MdLib::IDistributeConsumer<TestData>*)consumer);

    // 注册消费者
    bool ret = center.RegistDistributeConsumer<TestData>(consumerPtr);
    assert(ret);
    assert(!center.RegistDistributeConsumer<TestData>(consumerPtr));

    // 生产消费测试
    producer->ProduceOne();
    consumer->Consume();
    consumer->Consume();
    producer->ProduceOne();
    consumer->Consume();

    int maxSize = producer->GetContainer()->GetMaxSize();
    // 满配生产
    for (size_t i = 0; i <= maxSize; i++)
    {
        producer->ProduceOne();
    }

    for (size_t i = 0; i <= maxSize; i++)
    {
        consumer->Consume();
    }

    // 
    ret = center.DeRegistDistributeConsumer<TestData>(consumerPtr);
    assert(ret);
    consumer->Consume();
    return;
}
