#pragma once
namespace MdLib {
	class TypeCheckUtil {

	public:
		template<typename _parent, typename _child>
		static void AssertIsDelivedType();
		// static_assert(std::is_convertible<Derived*, Base*>::value, "Derived must inherit Base as public");
	};

	template<typename _parent, typename _child>
	inline void TypeCheckUtil::AssertIsDelivedType()
	{
		static_assert(std::is_convertible<_child*, _parent*>::value);
	}
}