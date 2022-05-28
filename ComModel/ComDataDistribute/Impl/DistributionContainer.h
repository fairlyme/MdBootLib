#pragma once
// sys header
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>

#include "../Interface/IDistributeContainer.h"

namespace MdLib {
	template<class DataType>
	class DistributionContainer : public IDistributeContainer<DataType> {
	protected:
		std::mutex _queueMtx;
		std::queue<std::shared_ptr<DataType>> _queue;

		std::condition_variable _dataReadyCv;
		std::condition_variable _dataDequedCv;

		int _maxSize;
		std::string _productKey;

	public:
		DistributionContainer(std::string productKey, int maxSize);

		// ��ȡ����������
		virtual std::string GetProductKey() override;

		// �����п�
		virtual bool IsEmpty() override;

		// ��������
		virtual bool IsFull() override;

		// �������ռ�
		virtual int GetMaxSize() override;

		// ���
		virtual bool Put(std::shared_ptr<DataType> item) override;

		// ����
		virtual bool Take(std::shared_ptr<DataType>& item) override;

		// �ȴ����� / �ȴ�����λ�ÿ���
		virtual void WaitDataDequeue(std::unique_lock<std::mutex> lk)  override;
		virtual void WaitDataDequeue(std::unique_lock<std::mutex> lk, int ms)  override;

		// �ȴ���� / �ȴ���������
		virtual void WaitDataEnqueue(std::unique_lock<std::mutex> lk)  override;
		virtual void WaitDataEnqueue(std::unique_lock<std::mutex> lk, int ms)  override;
	};

	template<class DataType>
	DistributionContainer<DataType>::DistributionContainer(std::string productKey, int maxSize)
		: _productKey(productKey), _maxSize(maxSize)
	{
	}

	template<class DataType>
	std::string DistributionContainer<DataType>::GetProductKey()
	{
		return _productKey;
	}

	template<class DataType>
	bool MdLib::DistributionContainer<DataType>::IsEmpty()
	{
		return _queue.size() == 0;
	}

	template<class DataType>
	bool DistributionContainer<DataType>::IsFull()
	{
		return _queue.size() >= _maxSize;
	}
	template<class DataType>
	int DistributionContainer<DataType>::GetMaxSize()
	{
		return _maxSize;
	}
	template<class DataType>
	bool DistributionContainer<DataType>::Put(std::shared_ptr<DataType> item)
	{
		if (IsFull()) {
			return false;
		}
		else
		{
			std::lock_guard<std::mutex> guard(_queueMtx);
			_queue.push(item);
			return true;
		}
	}

	template<class DataType>
	bool DistributionContainer<DataType>::Take(std::shared_ptr<DataType>& item)
	{
		if (!IsEmpty()) {
			std::lock_guard<std::mutex> guard(_queueMtx);

			item = _queue.front();
			_queue.pop();

			return true;
		}
		return false;
	}

	template<class DataType>
	void DistributionContainer<DataType>::WaitDataDequeue(std::unique_lock<std::mutex> lk)
	{
		if (IsFull()) {
			_dataDequedCv.wait(lk);
		}
		return;
	}

	template<class DataType>
	void DistributionContainer<DataType>::WaitDataDequeue(std::unique_lock<std::mutex> lk, int ms)
	{
		if (IsFull()) {
			_dataDequedCv.wait_for(lk, std::chrono::milliseconds(ms));
		}
		return;
	}

	template<class DataType>
	void DistributionContainer<DataType>::WaitDataEnqueue(std::unique_lock<std::mutex> lk)
	{
		if (IsEmpty()) {
			_dataReadyCv.wait(lk);
			return;
		}
		return;
	}
	template<class DataType>
	void DistributionContainer<DataType>::WaitDataEnqueue(std::unique_lock<std::mutex> lk, int ms)
	{
		if (IsEmpty()) {
			_dataReadyCv.wait_for(lk, std::chrono::milliseconds(ms));
			return;
		}
		return;
	}
}

