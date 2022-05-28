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
	/// BufferNode 导出
	/// </summary>
	struct BufferNode
	{
		const uint32_t blockID;
		const uint32_t bufferLen;
		const uint8_t* buffer;

		BufferNode(const uint32_t blockID, const uint32_t bufSize, const uint8_t* buf);
	};

	/// <summary>
	/// BufferNode 内部使用
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
	/// Buffer pool 块
	/// </summary>
	class BufferPoolBlock {
	private:
		uint8_t* _buf;

		/// <summary>
		/// 块ID
		/// </summary>
		const uint32_t _blockID;
		/// <summary>
		/// 块大小
		/// </summary>
		const uint32_t _bufferSize;

		/// <summary>
		/// 头
		/// </summary>
		BufferNodeInternal* _head;

		/// <summary>
		/// 尾
		/// </summary>
		BufferNodeInternal* _tail;

		/// <summary>
		/// node 表
		/// </summary>
		std::unordered_map<const uint8_t*, BufferNodeInternal*> _nodes;


	public:
		/// <summary>
		/// 构建
		/// </summary>
		/// <param name="bufferSize">缓存池块的大小</param>
		BufferPoolBlock(const uint32_t blockID, uint32_t bufferSize);

		~BufferPoolBlock();

		/// <summary>
		/// 分配内存 单位 byte
		/// </summary>
		/// <returns>如果不满足分配条件 返回nullptr</returns>
		BufferNode* AllocNode(uint32_t size);

		int GetBufSize();

		int NodeCount();

		/// <summary>
		/// 归还内存
		/// </summary>
		/// <param name="bufNode">node</param>
		/// <returns>非块内内存</returns>
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