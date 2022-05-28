#pragma once
#include <unordered_map>

#include "../../ComDef/ComDef.h"
#include "../../Utils/TypeUtil/TypeUtils.h"

#include "../../../ExternalDep/Singleton/Singleton.h"

#include "../Interface/IBufProvider.h"

#include "BufferBase.h"


namespace MdLib {
	class BufferManageCenter :public Singleton<BufferManageCenter> , public IBufManager{
		friend class Singleton<BufferManageCenter>;

	private:
		std::unordered_map<std::string, IBufProvider*> _providers;

	private:
		BufferManageCenter();

		/// <summary>
		/// 注册已知的Buf提供对象
		/// </summary>
		void RegistAllDefaultProvider();

	protected:
		bool RegistProviderInternal(IBufProvider* provider);

	public:

		template<class _BufProviderCls>
		bool RegistProvider();

		bool RegistProvider(std::unique_ptr<IBufProvider> provider);	

		IBufProvider* GetProvider(IBufDev* dev);
		IBufProvider* GetProvider(std::string devKey);

		/// <summary>
		/// 创建buffer
		/// </summary>
		/// <param name="dev">目标设备</param>
		/// <param name="createParam">创建参数</param>
		/// <returns>创建成功后的缓存</returns>
		std::unique_ptr<IBuf> CreateBuf(IBufDev* dev, BufferDataType dataType,int units);

		/// <summary>
		/// 创建buffer
		/// </summary>
		/// <param name="dev">目标设备</param>
		/// <param name="createParam">创建参数</param>
		/// <returns>创建成功后的缓存</returns>
		std::unique_ptr<IBuf> CreateBuf(std::string devKey, BufferDataType dataType, int units);

		/// <summary>
		/// 获取一个dev
		/// </summary>
		/// <param name="key"></param>
		/// <param name="bufPtr"></param>
		/// <returns></returns>
		bool TryGetDev(std::string key, IBufDev*& bufPtr);
	};

	template<class _BufProviderCls>
	inline bool BufferManageCenter::RegistProvider() {
		TypeCheckUtil::AssertIsDelivedType<IBufProvider, _BufProviderCls>();

		_BufProviderCls* providerPtr = new _BufProviderCls();

		return RegistProviderInternal(providerPtr);
	}
}