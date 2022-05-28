#pragma once
namespace MdLib {
	class IBufCopyParam {
	public:
		virtual int CopyLen() = 0;
		virtual int SrcMaxLen() = 0;
		virtual int SrcOffset() = 0;
		virtual int DstOffset() = 0;
		virtual int DstMaxLen() = 0;
	};
}