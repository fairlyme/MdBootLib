#pragma once
#include <memory>

#include "../Interface/IBuf.h"

#include <stdexcept>

namespace MdLib {
	class Buf : public IBuf {
		friend class BufferManageCenter;
		void* _buffer;
		BufferDataType _dataType;
		int _unitLen;
		int _units;

	private:
		Buf();
		void UpdateInfo(void* buffer, BufferDataType dataType = BufferDataType::DATA_UNKONWN, int unitLen = -1, int unitCount = -1);

	public:
		// Í¨¹ý IBuf ¼Ì³Ð
		virtual BufferDataType GetDataType() override;

		virtual void* GetBuffer() override;

		virtual int GetBufferBytes() override;

		virtual int GetBufUnitLen() override;

		virtual int GetBufUnits() override;
	};
}