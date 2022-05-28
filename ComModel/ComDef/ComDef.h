#pragma once
#include <string>

namespace MdLib {
	// buffer 数据类型
	enum BufferDataType
	{
		INT_8 = 0,
		INT_16 = 1,
		INT_32 = 2,
		INT_64 = 3,
		FLOAT_16 = 4,
		FLOAT_32 = 5,
		FLOAT_64 = 6,
		DATA_UNKONWN = 7,
		DATA_MAX_VAL = 7
	};

	constexpr auto CPU_DEV_NAME = "CPU";
}