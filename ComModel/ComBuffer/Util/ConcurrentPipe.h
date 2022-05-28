#pragma once
#include <queue>
#include <mutex>
#include <assert.h>
#include <functional>
#include <condition_variable>
namespace MdLib {
	template <class T>
	class ConcurrentPipe
	{
	public:
		const int MaxQueueSize;

	protected:
		std::mutex _handMutex;
		std::queue<T> _queue;

		std::condition_variable _dataReadyCv;
		std::condition_variable _dataDequedCv;

		std::function<void(T)> _onData;

		bool _markDropAll{ false };

	public:
		ConcurrentPipe(int maxQueueSize) : MaxQueueSize(maxQueueSize)
		{
		}

		~ConcurrentPipe()
		{
			_dataReadyCv.notify_all();
			_dataDequedCv.notify_all();
		}

		void MarkDropAll(bool dropAll) {
			_markDropAll = dropAll;
		}

		bool Enqueue(T item)
		{
			if (_markDropAll) {
				return true;
			}

			_handMutex.lock();

			if (_queue.size() >= MaxQueueSize) {
				_handMutex.unlock();
				return false;
			}

			_queue.push(item);
			_handMutex.unlock();

			_dataReadyCv.notify_one();

			return true;
		}

		bool Enqueue(T* item, int count)
		{
			if (_markDropAll) {
				return true;
			}

			_handMutex.lock();

			if (_queue.size() + count > MaxQueueSize) {
				_handMutex.unlock();
				return false;
			}

			for (int i = 0; i < count; i++)
			{
				assert(item[i] != nullptr);
				_queue.push(item[i]);
			}

			_handMutex.unlock();

			if (_onData) {
				for (int i = 0; i < count; i++)
				{
					_onData(item[i]);
				}
			}

			_dataReadyCv.notify_all();
			return true;
		}

		bool TryDequeue(T& item)
		{
			_handMutex.lock();
			if (_queue.empty()) {
				_handMutex.unlock();
				return false;
			}

			item = _queue.front();
			_queue.pop();

			_handMutex.unlock();

			if (_onData) {
				_onData(item);
			}

			_dataDequedCv.notify_one();
			return true;
		}

		bool TryDequeue(T* itemBuf, int& count)
		{
			if (count <= 0) {
				count = 0;
				return false;
			}

			_handMutex.lock();
			if (_queue.empty()) {
				_handMutex.unlock();
				count = 0;
				return false;
			}

			int tmpCount = 0;
			for (size_t i = 0; i < count; i++)
			{
				if (_queue.empty()) {
					break;
				}

				itemBuf[i] = _queue.front();
				_queue.pop();
				tmpCount += 1;
			}

			count = tmpCount;
			_handMutex.unlock();

			_dataDequedCv.notify_all();

			return count > 0;
		}

		void Clear() {
			_handMutex.lock();
			while (!_queue.empty())
			{
				_queue.pop();
			}
			_handMutex.unlock();
			_dataReadyCv.notify_all();
			_dataDequedCv.notify_all();
		}

		bool HasSpace()
		{
			return HasSpace(1);
		}

		bool HasSpace(int count) {
			if (count <= 0) {
				return false;
			}

			return MaxQueueSize >= _queue.size() + count;
		}

		int Count() {
			return _queue.size();
		}

		/// <summary>
		/// 等待,直到数据被送入被送入
		/// </summary>
		/// <param name="lk"></param>
		void WaitData(std::unique_lock<std::mutex> lk) {
			if (Count() > 0) {
				return;
			}
			return _dataReadyCv.wait(lk);
		}

		/// <summary>
		/// 等待,直到数据被送入或者超时
		/// </summary>
		std::cv_status WaitForData(std::unique_lock<std::mutex>& lk, int ms) {
			int curSize = Count();
			if (curSize > 0) {
				return std::cv_status::no_timeout;
			}

			return _dataReadyCv.wait_for(lk, std::chrono::milliseconds(ms));
		}

		void WaitDataDequeue(std::unique_lock<std::mutex>& lk) {
			if (Count() == 0) {
				// 没有数据 则永远不会发生dequeue
				return;
			}

			return _dataDequedCv.wait(lk);
		}

		std::cv_status WaitForDataDequeue(std::unique_lock<std::mutex>& lk, int ms) {
			if (Count() == 0) {
				// 没有数据 则永远不会发生dequeue
				return std::cv_status::no_timeout;
			}
			return _dataDequedCv.wait_for(lk, std::chrono::milliseconds(ms));
		}

		bool SetOnDataCallback(std::function<void(T)> callback) {
			if (!callback) {
				return false;
			}

			if (_onData) {
				return false;
			}

			_onData = callback;
		}

		void ResetOnDataCallback() {
			_onData = nullptr;
		}
	};
}