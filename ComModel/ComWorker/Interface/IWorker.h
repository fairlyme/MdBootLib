#pragma once
// sys headers

// module headers 
#include "../ModuleComDef/ComDef.h"

namespace MdLib {
	class IWorker {
	public:
		/// <summary>
		/// ʵ�� �����߼�, ����Ҫ������״̬
		/// ����һ��, ������Ҫ����̫��ʱ��
		/// </summary>
		virtual void DoProcessOnce() = 0;

		/// <summary>
		/// �����˳�
		/// </summary>
		virtual void OnProcessStop() = 0;

		/// <summary>
		/// ����ʼ
		/// </summary>
		virtual void OnProcessStart() = 0;

		/// <summary>
		/// work ����
		/// </summary>
		/// <returns></returns>
		virtual std::string What() = 0;

	};
}