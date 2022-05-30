#pragma once
namespace MdLib {
	class TypeCheckUtil {

	public:
		// 检查chile继承自parent,编译期间检查
		template<typename _parent, typename _child>
		static void AssertIsDelivedType();};

	template<typename _parent, typename _child>
	inline void TypeCheckUtil::AssertIsDelivedType()
	{
		static_assert(std::is_convertible<_child*, _parent*>::value);
	}
}