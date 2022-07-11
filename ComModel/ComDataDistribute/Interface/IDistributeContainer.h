#pragma once
#include<string>
#include <memory>
#include <mutex>

#include "IProductContainer.h"

namespace MdLib {

	// �����ߺ�������ģʽ����
	template<class T>
	class IDistributeContainer : public IProductContainer {
	public:
		// ���
		virtual bool Put(std::shared_ptr<T> item) = 0;

		// ����
		virtual bool Take(std::shared_ptr<T>& item) = 0;


		// �ȴ����� / �ȴ�����λ�ÿ���
		virtual void WaitDataDequeue(std::unique_lock<std::mutex> lk) = 0;
		virtual void WaitDataDequeue(std::unique_lock<std::mutex> lk, int ms) = 0;

		// �ȴ���� / �ȴ���������
		virtual void WaitDataEnqueue(std::unique_lock<std::mutex> lk) = 0;
		virtual void WaitDataEnqueue(std::unique_lock<std::mutex> lk, int ms) = 0;
	};

}