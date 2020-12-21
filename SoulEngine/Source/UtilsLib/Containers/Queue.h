/*
FIFO container.
@file Queue.h
@author Jacob Peterson
@version 1 12/20/20
*/

#pragma once

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	/*
	A Queue is FIFO container that supports simple operations such as Pop(),
	Push() and Peek() to manage the elements it contains. This is not a 
	thread-safe container, for a thread-safe implementation, check
	UtilsLib/Threads/ThreadSafeQueue.h.
	*/
	template <class T>
	class Queue
	{
	public:
		Queue(ArraySize uiCapacity = 32);

		/*
		Returns an immutable reference to the element at the front of this
		queue.

		@return Reference to the first element in the queue.
		*/
		const T& Peek() const;

		/*
		Removes and returns the element at the front of the queue.

		@return Element at the front of the queue.
		*/
		T Pop();

		/*
		Adds the provided element to the back of the queue.

		@param oElement - The element to be added to the front of the queue.
		*/
		void Push(const T& oElement);
		
		/*
		Adds the provided element to the back of the queue.

		@param oElement - The element to be added to the front of the queue.
		*/
		void Push(T&& oElement);

		/*
		Gets the maximum number of elements that can be stored in this queue.

		@return ArraySize containing the maximum queue capacity.
		*/
		const ArraySize& GetCapacity() const;

		/*
		Gets the current number of elements that are stored in this queue.

		@return ArraySize containing the current number of elements in this queue.
		*/
		const ArraySize& GetLength() const;

	private:
		UniqueHandle<T> _hMemory; // Handle to the memory block this queue uses.
		Index _uiHead; // The front of the queue.
		Index _uiTail; // The element after the last element.
		const ArraySize _cuiCapacity; // The maximum number of elements in the queue.
		ArraySize _uiLength; // The current number of elements in the queue.
	};

	template <class T>
	Queue<T>::Queue(ArraySize uiCapacity) :
		_hMemory(MemoryManager::AllocateArray<T>(uiCapacity)),
		_uiHead(0),
		_uiTail(0),
		_cuiCapacity(uiCapacity),
		_uiLength(0)
	{

	}

	template <class T>
	const T& Queue<T>::Peek() const
	{
		Assert(_uiLength > 0);
		return _hMemory[_uiHead];
	}

	template <class T>
	T Queue<T>::Pop()
	{
		Assert(_uiLength > 0);
		--_uiLength;
		return _hMemory[_uiHead++];
	}

	template <class T>
	void Queue<T>::Push(const T& oElement)
	{
		Assert(_uiLength < _cuiCapacity);

		_hMemory[_uiTail] = oElement;
		++_uiLength;
		_uiTail = (_uiTail + 1) % _cuiCapacity;
	}

	template <class T>
	void Queue<T>::Push(T&& oElement)
	{
		Assert(_uiLength < _cuiCapacity);

		_hMemory[_uiTail] = oElement;
		++_uiLength;
		_uiTail = (_uiTail + 1) % _cuiCapacity;
	}

	template <class T>
	const ArraySize& Queue<T>::GetCapacity() const
	{
		return _cuiCapacity;
	}
	
	template <class T>
	const ArraySize& Queue<T>::GetLength() const
	{
		return _uiLength;
	}
}