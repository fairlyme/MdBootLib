#pragma once
#include <string>

#include "../../ComDef/ComDef.h"
namespace MdLib {
	class IBufDev {
	public:
		virtual std::string DevKey() = 0;

		virtual int GetUnitLenOfDataType(BufferDataType type) = 0;
	};
}