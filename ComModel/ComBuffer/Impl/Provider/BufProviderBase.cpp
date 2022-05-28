
#include "../BufDev.h"
#include "BufProviderBase.h"

namespace MdLib {
	void BufProviderBase::SetDevDataTypeUnitLen(BufferDataType type, int len) {
		if (len < 0) {
			throw std::logic_error("len too small.");
		}

		if (type < 0 || type >= BufferDataType::DATA_MAX_VAL) {
			throw std::logic_error("invalid type value.");
		}

		_bufDev->defaultUnitSize[type] = len;
	}
	BufProviderBase::BufProviderBase(std::string devKey) {
		_bufDev = new BufDev(devKey);
	}
	BufProviderBase::~BufProviderBase() {
		delete _bufDev;
	}

	IBufDev* BufProviderBase::GetDevice()
	{
		return _bufDev;
	}
}