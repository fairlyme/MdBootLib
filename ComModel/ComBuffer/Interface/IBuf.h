#pragma once
#include <memory>
#include "../../ComDef/ComDef.h"
#include "IBufDev.h"

namespace MdLib {

	class IBuf {
	public:
		// 获取数据类型
		virtual BufferDataType GetDataType() = 0;
		// 获取buffer指针
		virtual void* GetBuffer() = 0;
		// 获取buffer长度
		virtual int GetBufferBytes() = 0;
		// buffer 单位长度
		virtual int GetBufUnitLen() = 0;
		// buffer 单位数量
		virtual int GetBufUnits() = 0;
	};


	class IBufManager {
	public:
		/// <summary>
		/// 创建 buffer 
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
		/// 创建 buffer 
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

	// 实际类型的 buf
	class IBufContainer {
	public:
		// 获取buffer
		virtual IBuf* GetBuffer() = 0;

		// 获取设备
		virtual IBufDev* GetDevice() = 0;

		// 获取所属设备名
		virtual std::string GetDeviceKey() = 0;

		// 移动到设备
		virtual bool ToDev(std::string devKey) = 0;

		// 移动到cpu
		virtual bool ToCpu() = 0;

		// 拷贝到其他Container
		virtual bool CopyTo(IBufContainer* buf) = 0;
	};
}