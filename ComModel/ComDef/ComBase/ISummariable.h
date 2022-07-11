#pragma once
#include <string>

namespace MdLib {
	class ISummariable {
	public:
		// summary
		virtual std::string Summary() = 0;
	};
}