#include "BufferBase.h"

namespace MdLib {
	Buf::Buf()
	{
	}
	
	void Buf::UpdateInfo(void* buffer, BufferDataType dataType, int unitLen, int unitCount) {
		if (buffer != nullptr) {
			if (dataType == BufferDataType::DATA_UNKONWN || unitLen < 0 || unitCount < 0) {
				throw std::logic_error("invalid param values.");
				return;
			}
		}

		_buffer = buffer;
		_dataType = dataType;
		_unitLen = unitLen;
		_units = unitCount;
	}

	BufferDataType Buf::GetDataType()
	{
		return _dataType;
	}

	void* Buf::GetBuffer()
	{
		return _buffer;
	}

	int Buf::GetBufferBytes()
	{
		return _unitLen * _units;
	}

	int Buf::GetBufUnitLen()
	{
		return _unitLen;
	}

	int Buf::GetBufUnits()
	{
		return _units;
	}
}