#pragma once
#include<string>

namespace MdLib {
	class IProductContainer {
	public:
		// 获取容器主题名
		virtual std::string GetProductKey() = 0;

		// 容器判空
		virtual bool IsEmpty() = 0;

		// 容器判满
		virtual bool IsFull() = 0;

		// 容器最大空间
		virtual int GetMaxSize() = 0;
	};
}