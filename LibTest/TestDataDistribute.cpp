#include "TestDataDistribute.h"

#include "ComModel/ComDataDistribute/ComDataExchange.h"
#include <assert.h>

void TestDataDistribution() {
    // ��ȡ �ַ����� ʵ�� 
    MdLib::DistributionCenter& center = MdLib::DistributionCenter::getInstance();

    // ����������
    TestProcuder *producer = new TestProcuder();
    std::shared_ptr<MdLib::IDistributeProducer<TestData>> producersPtr 
        = std::shared_ptr<MdLib::IDistributeProducer<TestData>>((MdLib::IDistributeProducer<TestData>*)producer);

    // ע��������
    assert(center.RegistDistributeProducer<TestData>(producersPtr));
    assert(!center.RegistDistributeProducer<TestData>(producersPtr));

    // ����������
    TestConsumer* consumer = new TestConsumer();
    std::shared_ptr<MdLib::IDistributeConsumer<TestData>> consumerPtr
        = std::shared_ptr<MdLib::IDistributeConsumer<TestData>>((MdLib::IDistributeConsumer<TestData>*)consumer);

    // ע��������
    bool ret = center.RegistDistributeConsumer<TestData>(consumerPtr);
    assert(ret);
    assert(!center.RegistDistributeConsumer<TestData>(consumerPtr));

    // �������Ѳ���
    producer->ProduceOne();
    consumer->Consume();
    consumer->Consume();
    producer->ProduceOne();
    consumer->Consume();

    int maxSize = producer->GetContainer()->GetMaxSize();
    // ��������
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
