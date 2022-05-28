#pragma once

#include "../Interface/IBuf.h"

namespace MdLib {
	class Buffer : public IBufContainer {
	private:
		std::unique_ptr<IBuf> _bufHolded;
		std::string devKey;
		IBufDev* devPtr;

	public:
		Buffer(std::string dev,BufferDataType dataType,int bufUnits);
		~Buffer();

		// Í¨¹ý IBufContainer ¼Ì³Ð
		virtual IBuf* GetBuffer() override;
		virtual IBufDev* GetDevice() override;
		virtual std::string GetDeviceKey() override;
		virtual bool ToDev(std::string devKey) override;
		virtual bool ToCpu() override;
		virtual bool CopyTo(IBufContainer* buf) override;
	};
}

