#include "BufDev.h"
namespace MdLib {
	BufDev::BufDev(const char* devKey) :_devKey(devKey) {

	}
	BufDev::BufDev(std::string devKey) : _devKey(devKey) {
	}
	std::string BufDev::DevKey() {
		return _devKey;
	}
	
	int BufDev::GetUnitLenOfDataType(BufferDataType type) {
		if (type < 0 || type >= BufferDataType::DATA_MAX_VAL) {
			throw std::logic_error("invalid param");
		}

		return defaultUnitSize[type];
	}
}