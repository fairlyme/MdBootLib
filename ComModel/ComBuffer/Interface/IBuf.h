#pragma once
#include <memory>
#include "../../ComDef/ComDef.h"
#include "IBufDev.h"

namespace MdLib {

	class IBuf {
	public:
		// ��ȡ��������
		virtual BufferDataType GetDataType() = 0;
		// ��ȡbufferָ��
		virtual void* GetBuffer() = 0;
		// ��ȡbuffer����
		virtual int GetBufferBytes() = 0;
		// buffer ��λ����
		virtual int GetBufUnitLen() = 0;
		// buffer ��λ����
		virtual int GetBufUnits() = 0;
	};


	class IBufManager {
	public:
		/// <summary>
		/// ���� buffer 
		/// </summary>
		/// <param name="devKey"></param>
		/// <param name="dataType"></param>
		/// <param name="allocUnits"></param>
		/// <returns></returns>
		virtual std::unique_ptr<IBuf> CreateBuf(
			std::string devKey, 
			BufferDataType dataType, 
			int allocUnits
		) = 0;

		/// <summary>
		/// ���� buffer 
		/// </summary>
		/// <param name="devKey"></param>
		/// <param name="dataType"></param>
		/// <param name="allocUnits"></param>
		/// <returns></returns>
		virtual std::unique_ptr<IBuf> CreateBuf(
			IBufDev* dev,
			BufferDataType dataType,
			int allocUnits
		) = 0;
	};

	// ʵ�����͵� buf
	class IBufContainer {
	public:
		// ��ȡbuffer
		virtual IBuf* GetBuffer() = 0;

		// ��ȡ�豸
		virtual IBufDev* GetDevice() = 0;

		// ��ȡ�����豸��
		virtual std::string GetDeviceKey() = 0;

		// �ƶ����豸
		virtual bool ToDev(std::string devKey) = 0;

		// �ƶ���cpu
		virtual bool ToCpu() = 0;

		// ����������Container
		virtual bool CopyTo(IBufContainer* buf) = 0;
	};
}