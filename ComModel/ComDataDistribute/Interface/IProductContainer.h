#pragma once
#include<string>

namespace MdLib {
	class IProductContainer {
	public:
		// ��ȡ����������
		virtual std::string GetProductKey() = 0;

		// �����п�
		virtual bool IsEmpty() = 0;

		// ��������
		virtual bool IsFull() = 0;

		// �������ռ�
		virtual int GetMaxSize() = 0;
	};
}