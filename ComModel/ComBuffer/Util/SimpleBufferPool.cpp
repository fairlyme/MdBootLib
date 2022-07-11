#include "SimpleBufferPool.h"

namespace MdLib {
	BufferNode::BufferNode(const uint32_t blockID, const uint32_t bufSize, const uint8_t* buf)
		: blockID(blockID), bufferLen(bufSize), buffer(buf)
	{

	}

	BufferNodeInternal::BufferNodeInternal(const uint8_t* buf, const uint32_t bufSize, const uint32_t distance, BufferNodeInternal* preNode, BufferNodeInternal* nextNode)
		: buffer(buf), bufferSize(bufSize), distance(distance), pre(preNode), next(nextNode)
	{
	}

	/// <summary>
	/// 构建
	/// </summary>
	/// <param name="bufferSize">缓存池块的大小</param>
	BufferPoolBlock::BufferPoolBlock(const uint32_t blockID, uint32_t bufferSize)
		: _blockID(blockID), _bufferSize(bufferSize)
	{
		_buf = new uint8_t[bufferSize];

		_head = new BufferNodeInternal((const uint8_t*)_buf, 0, bufferSize, nullptr, nullptr);
		_tail = new BufferNodeInternal((const uint8_t*)(_buf + _bufferSize), 0, 0, _head, nullptr);

		_head->next = _tail;
	}

	BufferPoolBlock::~BufferPoolBlock() {
		for (std::unordered_map<const uint8_t*, BufferNodeInternal*>::iterator it = _nodes.begin(); it != _nodes.end(); it++)
		{
			delete it->second;
		}

		_nodes.clear();

		delete _head;
		delete _tail;

		_head = nullptr;
		_tail = nullptr;

		delete[] _buf;
	}

	/// <summary>
	/// 分配内存 单位 byte
	/// </summary>
	/// <returns>如果不满足分配条件 返回nullptr</returns>
	BufferNode* BufferPoolBlock::AllocNode(uint32_t size) {
		BufferNodeInternal* node = _head;

		// 找到大小合适的
		while (node->distance < size && node->next != nullptr)
		{
			node = node->next;
		}

		if (node->next == nullptr) {
			// 无空余内存
			return nullptr;
		}

		BufferNodeInternal* newNode = new BufferNodeInternal((node->buffer + node->bufferSize), size, node->distance - size, node, node->next);

		node->next->pre = newNode;
		node->next = newNode;

		node->distance = 0;

		// 缓存
		_nodes[newNode->buffer] = newNode;

		return new BufferNode(_blockID, size, newNode->buffer);
	}

	int BufferPoolBlock::GetBufSize()
	{
		return _bufferSize;
	}

	int BufferPoolBlock::NodeCount() {
		return (int)_nodes.size();
	}

	/// <summary>
	/// 归还内存
	/// </summary>
	/// <param name="bufNode">node</param>
	/// <returns>非块内内存</returns>
	bool BufferPoolBlock::ReleaseNode(BufferNode* bufNode) {
		if (bufNode == nullptr || bufNode->blockID != _blockID) {
			return false;
		}

		std::unordered_map<const uint8_t*, BufferNodeInternal*>::iterator it = _nodes.find(bufNode->buffer);
		if (it == _nodes.end()) {
			return false;
		}

		BufferNodeInternal* curNode = it->second;

		curNode->pre->distance += curNode->bufferSize + curNode->distance;
		curNode->pre->next = curNode->next;

		if (curNode->next != nullptr) {
			curNode->next->pre = curNode->pre;
		}

		_nodes.erase(it);
		delete bufNode;
		delete curNode;

		return true;
	}

	bool BufferPoolBlock::IsEmpty() {
		return _head->next == _tail;
	}

	BufferNode* SimpleBufferPool::_AllocNode(uint32_t size)
	{
		if (size > _singleBlockSize) {
			return nullptr;
		}

		std::unordered_map<uint32_t, BufferPoolBlock*> ::iterator it;
		BufferNode* ret;

		for (it = _blocks.begin(); it != _blocks.end(); it++) {
			ret = it->second->AllocNode(size);
			if (ret != nullptr) {
				return ret;
			}
		}

		size_t id = _blocks.size();
		if (id >= _maxBlockCount) {
			return nullptr;
		}
		BufferPoolBlock* newBlock = new BufferPoolBlock(id, _singleBlockSize);

		_blocks[id] = newBlock;

		ret = newBlock->AllocNode(size);

		return ret;
	}

	bool SimpleBufferPool::_ReleaseNode(BufferNode* node)
	{
		auto it = _blocks.find(node->blockID);
		if (it != _blocks.end()) {
			if (it->second->ReleaseNode(node)) {
				if (it->second->IsEmpty()) {
					delete it->second;
					_blocks.erase(it);
				}
				return true;
			}
			return false;
		}
		return false;
	}

	int SimpleBufferPool::_NodeCount()
	{
		int count = 0;
		for (auto it = _blocks.begin(); it != _blocks.end(); it++) {
			count += it->second->NodeCount();
		}

		return count;
	}

	SimpleBufferPool::SimpleBufferPool(uint32_t singleBlockSize, uint32_t maxBlockCount)
		: _singleBlockSize(singleBlockSize), _maxBlockCount(maxBlockCount)
	{

	}

	SimpleBufferPool::~SimpleBufferPool() {
		for (auto it = _blocks.begin(); it != _blocks.end(); it++) {
			delete it->second;
		}
		_blocks.clear();
	}

	BufferNode* SimpleBufferPool::AllocNode(uint32_t size) {
		_mutex.lock();
		BufferNode* ret = _AllocNode(size);
		_mutex.unlock();
		return ret;
	}

	std::shared_ptr<BufferNode> SimpleBufferPool::AllocNodeSafety(std::shared_ptr<SimpleBufferPool> pool, uint32_t size) {
		BufferNode* ret = pool->AllocNode(size);

		if (ret == nullptr) {
			return nullptr;
		}
		else
		{
			std::weak_ptr< SimpleBufferPool>  poolWeak = pool;
			return std::shared_ptr<BufferNode>(ret, [poolWeak](BufferNode* nodePtr) {
				std::shared_ptr<SimpleBufferPool> pool = poolWeak.lock();
				if (pool) {
					pool->ReleaseNode(nodePtr);
				}
				});
		}
	}

	bool SimpleBufferPool::ReleaseNode(BufferNode* node) {
		_mutex.lock();
		bool ret = _ReleaseNode(node);
		_mutex.unlock();
		return ret;
	}

	BufferNode** SimpleBufferPool::AllocNodePatch(uint32_t size, uint32_t count)
	{
		_mutex.lock();
		if (count <= 0) {
			return nullptr;
		}

		BufferNode** ret = new BufferNode * [count];

		unsigned int i;
		for (i = 0; i < count; i++)
		{
			ret[i] = nullptr;
		}

		bool sucess = true;
		for (i = 0; i < count; i++)
		{
			ret[i] = _AllocNode(size);
			if (ret[i] == nullptr) {
				// 释放已经分配的
				for (size_t j = 0; j < i; j++)
				{
					_ReleaseNode(ret[j]);
				}
				sucess = false;
				break;
			}
		}

		_mutex.unlock();

		if (sucess) {
			return ret;
		}
		else
		{
			delete[] ret;
			return nullptr;
		}
	}

	bool SimpleBufferPool::ReleaseNodePatch(BufferNode** node, uint32_t count)
	{
		_mutex.lock();
		bool ret = true;
		for (size_t i = 0; i < count; i++)
		{
			if (_ReleaseNode(node[i])) {
				ret = false;
			}
		}

		_mutex.unlock();
		return ret;
	}

	int SimpleBufferPool::NodeCount() {
		_mutex.lock();
		int nodeCount = _NodeCount();
		_mutex.unlock();
		return nodeCount;
	}

	int SimpleBufferPool::BlockCount() {
		return (int)_blocks.size();
	}
}