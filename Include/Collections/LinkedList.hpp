// File: LinkedList.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_LinkedList
#define _L_LinkedList
#include "Fundamental.hpp"
#include "../Exception.hpp"
#include "IList.hpp"

namespace LiongPlus
{
	/// <summary>
	/// This class is a component of LinkedList<T>.
	/// </summary>
	/// <note>
	/// LinkedList<T> is a structure storing data non-continuously. It seems not suitable for storing small objects.
	/// The GetNext() and GetPrevious() methods return raw pointers to another node and it may be nullptr when the current node is at the ends. To make your code concisely, when the availability of returned pointer is guaranteed, it is suggested to write:
	/// <c>auto& nextNode = *currentNode.Next();</c>
	/// Otherwise, you should check the availability of the returned pointer before refering it.
	/// </node>
	template<typename T>
	struct LinkedListNode
		: public Object
	{
		friend class LinkedList<T>;
	public:
		// Constructor
		
		LinkedListNode()
			, _Former(nullptr)
			, _Latter(nullptr)
			, _Value()
		{
		}
		LinkedListNode(T value)
			: _Former(nullptr)
			, _Latter(nullptr)
			, _Value(value)
		{
		}
		LinkedListNode(const LinkedListNode<T>& instance)
			: _Former(instance._Former)
			, _Latter(instance._Latter)
			, _Value(instance._Value)
		{
		}
		LinkedListNode(LinkedListNode<T>&& instance)
			: _Former(nullptr)
			, _Latter(nullptr)
			, _Value()
		{
			Swap(_Former, instance._Former);
			Swap(_Latter, instance._Latter);
			Swap(_Value, instance._Value);
		}
	private:
		LinkedListNode(LinkedListNode<T>& former, T value, LinkedListNode<T>& latter)
			: _Former(nullptr)
			, _Latter(nullptr)
			, _Value(value)
		{
			Bond(former, latter);
		}
	public:
		
		// Operator
		
		LinkedListNode<T>& operator=(const LinkedListNode<T>& instance)
		{
			_Former = instance._Former;
			_Latter = instance._Latter;
			_Value = instance._Value;
			
			return *this;
		}
		LinkedListNode<T>& operator=(LinkedListNode<T>&& instance)
		{
			Swap(_Former, instance._Former);
			Swap(_Latter, instance._Latter);
			Swap(_Value, instance._Value);
			
			return *this;
		}
		
		// Methods
		
		LinkedListNode<T>* GetNext()
		{
			return _Latter;
		}
		LinkedListNode<T>* GetNext(int length)
		{
			auto ptr = this;
			while (--length > 0 && ptr != nullptr)
				ptr = ptr->_Latter;
			return ptr;
		}
		
		LinkedListNode<T>* GetPrevious()
		{
			return _Former;
		}
		LinkedListNode<T>* GetPrevious(int length)
		{
			auto ptr = this;
			while (--length > 0 && ptr != nullptr)
				ptr = ptr->_Former;
			return ptr;
		}
		
		T& GetValue()
		{
			return _Value;
		}
	private:
		LinkedListNode<T>* _Former;
		LinkedListNode<T>* _Latter;
		T _Value;
		
		void Bond(LinkedListNode<T>& former, LinkedListNode<T>& latter)
		{
			former._Latter = &latter;
			latter._Former = &former;
			_Former = &former;
			_Latter = &latter;
			former._Latter = this;
			latter._Former = this;
		}
	};
	
	template<typename T>
	class LinkedList
		: public Object
		, public ICollection<T>
		, public IEnumerable<T>
		, public IList<T>
	{
	public:
		class Enumerator
			: public Object
			, public IEnumerator<T>
		{
		public:
			// Constructor
			
			Enumerator(LinkedListNode<T>* node)
				: _Node(node)
			{
			}
			Enumerator(const Enumerator& instance)
				: _Node(instance._Node)
			{
			}
			Enumerator(Enumerator&& instance)
				: _Node(nullptr)
			{
				Swap(_Node, instance._Node);
			}
			~Enumerator()
			{
			}
			
			// Operator
			
			Enumerator<T>& operator=(const Enumerator<T>& instance)
			{
				_Node = instance._Node;
				return *this;
			}
			Enumerator<T>& operator=(Enumerator<T>&& instance)
			{
				Swap(_Node, instance._Node);
			}
			
			// Method
			
			LinkedListNode<T>& CurrentNode() override
			{
				return *_Node;
			}
			
			bool MovePrevious()
			{
				if (_Node->_Former != nullptr)
				{
					_Node = _Node->_Former;
					return true;
				}
				else return false;
			}
			
			// IEnumerator<T>
			
			virtual T& Current() override
			{
				return _Node->_Value;
			}
			
			virtual bool Equals(IEnumerator<T>* value) override
			{
				if (value->_Node == _Node) return true;
				else return false;
			}
			
			virtual bool MoveNext() override
			{
				if (_Node->_Latter != nullptr)
				{
					_Node = _Node->_Latter;
					return true;
				}
				else return false;
			}
			
			// IDisposable
			
			virtual void Dispose() override
			{
			}
			
		private:
			LinkedListNode<T>* _Node;
		};
		
	public:
		
		// Constructor
		
		LinkedList()
			: _Begin(nullptr)
			, _End(nullptr)
		{
		}
		LinkedList(int count)
			: _Begin(nullptr)
			, _End(nullptr)
		{
		}
		LinkedList(const LinkedList<T>& instance)
			: _Begin(instance._Begin)
			: _End(instance._End)
		{
		}
		LinkedList(LinkedList<T>&& instance)
			: _Begin(nullptr)
			: _End(nullptr)
		{
			Swap(_Begin, instance._Begin);
			Swap(_End, instance._End);
		}
		
		// Operator
		
		LinkedList<T>& operator=(const LinkedList<T>& instance)
		{
			_Begin = instance._Begin;
		}
		LinkedList<T>& operator=(LinkedList<T>&& instance)
		{
			Swap(_Begin, instance._Begin);
		}
		
		// Method
		
		// Because a node may be in another linked list already and this implementation of node cannot retrieve the linked list (the end points cannot be modified). To prevent the occurence of unexcepted behaviors, the methods of adding, removing and inserting of node are abandoned.
		
		LinkedListNode<T>& AddAfter(LinkedListNode<T>& pos, T& value)
		{
			if (pos._Latter != nullptr) // If $pos is not the last node:
				return new LinkedListNode<T>(*(pos._Former), value, pos); // Simply bond the node to the nodes 'nearby'.
			else
			{
				// Let the new node be the first node.
				auto node = new LinkedListNode<T>(value);
				node->_Former = _End;
				_End = node;
				return node;
			}
		}
		
		LinkedListNode<T>& AddBefore(LinkedListNode<T>& pos, T& value)
		{
			if (pos._Former != nullptr)
				return new LinkedListNode<T>(pos, value, *(pos._Former)); 
			else
			{
				auto node = new LinkedListNode<T>(value);
				node->_Latter = _Begin;
				_Begin = node;
				return node;
			}
		}
		
		LinkedListNode<T>& AddFirst(T& value)
		{
			return AddBefore(*_Begin, value);
		}
		LinkedListNode<T>& AddFirst(LinkedListNode<T>& node)
		{
			return AddBefore(*_Begin, node);
		}
		
		LinkedListNode<T>& AddLast(T& value)
		{
			return AddAfter(*_End, value);
		}
		LinkedListNode<T>& AddLast(LinkedListNode<T>& node)
		{
			return AddAfter(*_End, node);			
		}
		
		int FindIndex()
		LinkedListNode<T>& FindNode
		
		// Static
		
		static void Splice(LinkedList<T>& dst, int dstIndex, LinkedList<T>& src, int srcIndex, int length)
		{
			// The process is generally:
			//	 INDEX ACCENDING DIRECTION >>>
			// SRC: ... _ _ _ A|B C|D _ _ _ ...
			//                  \ /
			// DST: ... _ _ _ _ E|F _ _ _ _ ...
			//	 A._Latter = D   B._Former = E
			//	 C._Latter = F   D._Former = A
			//	 E._Latter = B   F._Former = C
			auto dstFormer = dst._Begin->GetNext(dstIndex);
			assert(dstFormer == nullptr, "$dst or $dstIndex");
			auto dstLatter = dstFormer->_Latter;
			
			auto srcFormer = src._Begin->GetNext(srcIndex);
			assert(srcFormer == nullptr, "$dst or $srcIndex");
			auto srcLatter = srcFormer->Latter;
			
			if (srcFormer == src._Begin) // If B is the beginning of the source:
				src._Begin = srcLatter->_Latter; // Let D be the start of source.
			else // If B is not the beginning of the source:
				srcFormer->_Former->_Latter = srcLatter->_Latter; // A is accessable, connect A with D in the accending direction.
			
			if (srcLatter == src._End)
				src._End = srcFormer->_Former;
			else
				srcLatter->_Latter->_Former = srcFormer->_Former; // D
			
			dstFormer->_Latter = srcFormer; // E
			srcFormer->_Former = dstFormer; // B
			srcLatter->_Latter = dstLatter->_Former;// C
			dstLatter->_Former = srcLatter; // F
		}
		
		// IEnumerable<T>
		
		virtual Ptr<IEnumerator<T>> GetEnumerator() override
		{
			return new Enumerator(_Begin);
		}
		
		// ICollection<T>
		
		virtual int Add(T& value) override
		{
			auto ptr = &AddLast(value);
			int index = -1;
			
			while (ptr != nullptr)
			{
				ptr = ptr->_Former;
				++index;
			}
			return index;
		}
		
		virtual void Clear() override
		{
			auto ptr = _Begin;
			while (ptr != nullptr)
			{
				auto temp = ptr->_Latter;
				delete ptr;
				ptr = temp;
			}
			_Begin = _End = nullptr;
		}
		
		virtual bool Contains(T& value) override
		{
			auto ptr = _Begin;
			
			while (ptr != nullptr)
			{
				if (ptr->_Value == value)
				return true;
				ptr = ptr->_Former;
			}
			return false;
		}
		
		virtual 
		
		// IList<T>
		
		int GetCount()
		{
			auto ptr = _Begin;
			int count = 0;
			
			while (ptr != nullptr)
			{
				++count;
				ptr = ptr->_Latter;
			}
			
			return count;
		}
		
	private:
		LinkedListNode<T>* _Begin;
		LinkedListNode<T>* _End;
	};
};

#endif
