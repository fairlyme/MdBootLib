#include "BufferManager.h"
#include "Provider/CpuBufProvider.h"
#include "BufferBase.h"

namespace MdLib {
	BufferManageCenter::BufferManageCenter() {
		RegistAllDefaultProvider();
	}

	void BufferManageCenter::RegistAllDefaultProvider()
	{
		RegistProvider<CpuBufProvider>();
	}

	/// <summary>
	/// ע��provider
	/// </summary>
	/// <param name="provider">ע���providerָ��,������ new ������.�Ȳ��ᱻ�Զ��ͷŵ�����</param>
	/// <returns>ע����</returns>
	bool BufferManageCenter::RegistProviderInternal(IBufProvider* provider)
	{
		IBufDev* dev = provider->GetDevice();
		if (_providers.find(dev->DevKey()) == _providers.end()) {
			_providers[dev->DevKey()] = provider;
			return true;
		}
		else {
			throw std::logic_error("Repeat registration");
			return false;
		}
	}

	IBufProvider* BufferManageCenter::GetProvider(IBufDev* dev)
	{
		return GetProvider(dev->DevKey());
	}

	IBufProvider* BufferManageCenter::GetProvider(std::string devKey)
	{
		auto it = _providers.find(devKey);

		if (it == _providers.end()) {
			return nullptr;
		}
		return it->second;
	}

	/// <summary>
	/// ע��provider
	/// </summary>
	/// <param name="provider">ע�������ָ�� unique_ptr</param>
	/// <returns>ע����</returns>
	bool BufferManageCenter::RegistProvider(std::unique_ptr<IBufProvider> provider)
	{
		if (RegistProviderInternal(provider.get())) {
			provider.release(); // �ͷ�����Ȩ
			return true;
		}

		return false; // ��ʧprovider,�����ͷ�.
	}

	std::unique_ptr<IBuf> BufferManageCenter::CreateBuf(IBufDev* dev, BufferDataType dataType, int units)
	{
		IBufProvider* provider = GetProvider(dev);

		if (provider == nullptr) {
			return std::unique_ptr<IBuf>();
		}
		Buf* buf = new Buf();
		
		void* bufPtr = provider->CreateBuf(dataType, units);
		if (bufPtr == nullptr) {
			delete buf;
		}

		buf->UpdateInfo(buf, dataType, provider->GetDevice()->GetUnitLenOfDataType(dataType), units);

		return std::unique_ptr<IBuf>(buf);
	}

	std::unique_ptr<IBuf> BufferManageCenter::CreateBuf(std::string devKey, BufferDataType dataType, int units)
	{
		auto provider = GetProvider(devKey);

		if (provider != nullptr) {
			Buf* buf = new Buf();

			void* bufPtr = provider->CreateBuf(dataType, units);
			if (bufPtr == nullptr) {
				delete buf;
			}

			buf->UpdateInfo(bufPtr, dataType, provider->GetDevice()->GetUnitLenOfDataType(dataType), units);

			return std::unique_ptr<IBuf>(buf);
		}
		return std::unique_ptr<IBuf>();
	}


	bool BufferManageCenter::TryGetDev(std::string key, IBufDev*& devPtr)
	{
		IBufProvider* provider = GetProvider(key);

		if (provider == nullptr) {
			devPtr = nullptr;
			return false;
		}
		devPtr = provider->GetDevice();

		return true;
	}
}