#pragma once

#include "../Interface/IBufDev.h"
#include <stdexcept>


namespace MdLib {
	class BufDev : public IBufDev {
		friend class BufProviderBase;

		const std::string _devKey;
		int defaultUnitSize[BufferDataType::DATA_MAX_VAL] = {
			1,// INT_8
			2,// INT_16
			4,// INT_32
			8,// INT_64
			2,// FLOAT_16
			4,// FLOAT_32
			8,// FLOAT_64
		};
	private:
		BufDev(const char* devKey);
		BufDev(std::string devKey);


	public:

		virtual std::string DevKey() override;

		virtual int GetUnitLenOfDataType(BufferDataType type) override;
	};
}