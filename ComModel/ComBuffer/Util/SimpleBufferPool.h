#pragma once
#include <stdlib.h>
#include <assert.h>

#include <stdint.h>
#include <unordered_map>
#include <list>

#include <mutex>
#include <memory>

namespace MdLib {
	/// <summary>
	/// BufferNode ����
	/// </summary>
	struct BufferNode
	{
		const uint32_t blockID;
		const uint32_t bufferLen;
		const uint8_t* buffer;

		BufferNode(const uint32_t blockID, const uint32_t bufSize, const uint8_t* buf);
	};

	/// <summary>
	/// BufferNode �ڲ�ʹ��
	/// </summary>
	struct BufferNodeInternal {
		const uint8_t* buffer;
		const uint32_t bufferSize;
		uint32_t distance;

		BufferNodeInternal* pre;
		BufferNodeInternal* next;

	public:
		BufferNodeInternal(const uint8_t* buf, const uint32_t bufSize, const uint32_t distance, BufferNodeInternal* preNode, BufferNodeInternal* nextNode);
	};

	/// <summary>
	/// Buffer pool ��
	/// </summary>
	class BufferPoolBlock {
	private:
		uint8_t* _buf;

		/// <summary>
		/// ��ID
		/// </summary>
		const uint32_t _blockID;
		/// <summary>
		/// ���С
		/// </summary>
		const uint32_t _bufferSize;

		/// <summary>
		/// ͷ
		/// </summary>
		BufferNodeInternal* _head;

		/// <summary>
		/// β
		/// </summary>
		BufferNodeInternal* _tail;

		/// <summary>
		/// node ��
		/// </summary>
		std::unordered_map<const uint8_t*, BufferNodeInternal*> _nodes;


	public:
		/// <summary>
		/// ����
		/// </summary>
		/// <param name="bufferSize">����ؿ�Ĵ�С</param>
		BufferPoolBlock(const uint32_t blockID, uint32_t bufferSize);

		~BufferPoolBlock();

		/// <summary>
		/// �����ڴ� ��λ byte
		/// </summary>
		/// <returns>���������������� ����nullptr</returns>
		BufferNode* AllocNode(uint32_t size);

		int GetBufSize();

		int NodeCount();

		/// <summary>
		/// �黹�ڴ�
		/// </summary>
		/// <param name="bufNode">node</param>
		/// <returns>�ǿ����ڴ�</returns>
		bool ReleaseNode(BufferNode* bufNode);

		bool IsEmpty();
	};

	class SimpleBufferPool
	{
	private:
		const uint32_t _singleBlockSize;
		std::unordered_map<uint32_t, BufferPoolBlock*> _blocks;

		std::mutex _mutex;
		const uint32_t _maxBlockCount;
	private:
		BufferNode* _AllocNode(uint32_t size);
		bool _ReleaseNode(BufferNode* node);
		int _NodeCount();
	public:
		SimpleBufferPool(uint32_t blockSize, uint32_t maxBlockCount = 1u);

		~SimpleBufferPool();

		BufferNode* AllocNode(uint32_t size);

		static std::shared_ptr<BufferNode> AllocNodeSafety(std::shared_ptr<SimpleBufferPool> pool, uint32_t size);

		bool ReleaseNode(BufferNode* node);

		BufferNode** AllocNodePatch(uint32_t size, uint32_t count);

		bool ReleaseNodePatch(BufferNode** node, uint32_t count);

		int NodeCount();

		int BlockCount();
	};
}