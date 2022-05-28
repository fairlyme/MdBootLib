#pragma once
#include <vector>
#include <functional>
#include <assert.h>

namespace MdLib {
	template<typename T>
	struct CycleNode
	{
	private:
		bool processedFlag;
		bool processPlaned;
		bool setDataPlaned;
	public:
		const int id;
		const T dataItem;
		CycleNode* next;

		void MarkProcessed() {
			assert(CanProcess());
			processedFlag = true;
			processPlaned = false;
		}

		void MarkDataSeted() {
			assert(CanSetNew());
			processedFlag = false;
			setDataPlaned = false;
		}

		bool CanSetNew() {
			// 数据为空
			// 或者有数据已经处理了
			// 如果已经处理 有数据 
			return processedFlag;
		}

		bool CanProcess() {
			// 有数据 未处理
			return !processedFlag;
		}

		void MarkProcessPlan() {
			processPlaned = true;
		}

		bool ProcessPlaned() {
			return processPlaned;
		}


		void MarkDataSetPlan() {
			setDataPlaned = true;
		}

		bool SetDataPlaned() {
			return setDataPlaned;
		}

		CycleNode(int id, T dataItem)
			:id(id), dataItem(dataItem), processedFlag(true), next(nullptr), processPlaned(false), setDataPlaned(false)
		{

		}
	};

	template<typename T>
	static bool CreateCycleBuffer(int count, std::vector<CycleNode<T>*>& nodeCollect, CycleNode<T>*& _firstNode, std::function<CycleNode<T>* (int)> creator)
	{
		CycleNode<T>* curNode{ nullptr };
		CycleNode<T>* preNode{ nullptr };
		_firstNode = nullptr;
		for (int idx = 0; idx < count; idx++) {
			curNode = creator(idx);
			nodeCollect.push_back(curNode);

			if (_firstNode == nullptr) {
				// first
				_firstNode = curNode;
			}
			else
			{
				preNode->next = curNode;
			}

			preNode = curNode;
		}
		curNode->next = _firstNode;

		return true;
	}

	template<typename T>
	class CycleBuffer {
	private:
		std::vector<CycleNode<T>*> _allNode;
	public:
		const int nodeCount;
		CycleNode<T>* curNode;

		CycleBuffer(int count, std::function<CycleNode<T>* (int)> creator)
			: nodeCount(count) {
			assert(CreateCycleBuffer(nodeCount, _allNode, curNode, creator));
		}
	};
}