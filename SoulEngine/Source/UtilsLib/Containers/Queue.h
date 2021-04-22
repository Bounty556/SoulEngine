/*
FIFO container.
@file Queue.h
@author Jacob Peterson
@edited 4/21/21
*/

#pragma once

#include <Memory/MemoryManager.h>
#include <Memory/UniqueHandle.h>
#include <UtilsLib/CommonTypes.h>

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
		Queue(ArraySize capacity);
		Queue(Queue<T>&& otherQueue);

		Queue<T>& operator=(Queue<T>&& otherQueue);

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

		@param element - The element to be added to the front of the queue.
		*/
		void Push(const T& element);
		
		/*
		Adds the provided element to the back of the queue.

		@param element - The element to be added to the front of the queue.
		*/
		void Push(T&& element);

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

		Queue() = delete;
		Queue(const Queue<T>&) = delete;
		Queue<T>& operator=(const Queue<T>&) = delete;

	private:
		UniqueHandle<T> m_Memory; // Handle to the memory block this queue uses.
		Index m_Head; // The front of the queue.
		Index m_Tail; // The element after the last element.
		ArraySize m_Capacity; // The maximum number of elements in the queue.
		ArraySize m_Length; // The current number of elements in the queue.
	};

	template <class T>
	Queue<T>::Queue(ArraySize capacity) :
		m_Memory(MemoryManager::AllocateArray<T>(capacity)),
		m_Head(0),
		m_Tail(0),
		m_Capacity(capacity),
		m_Length(0)
	{

	}

	template <class T>
	Queue<T>::Queue(Queue<T>&& otherQueue) :
		m_Memory(std::move(otherQueue.m_Memory)),
		m_Head(otherQueue.m_Head),
		m_Tail(otherQueue.m_Tail),
		m_Capacity(otherQueue.m_Capacity),
		m_Length(otherQueue.m_Length)
	{
		otherQueue.m_Head = 0;
		otherQueue.m_Tail = 0;
		otherQueue.m_Length = 0;
	}

	template <class T>
	Queue<T>& Queue<T>::operator=(Queue<T>&& otherQueue)
	{
		m_Memory = std::move(otherQueue.m_Memory);
		m_Head = otherQueue.m_Head;
		m_Tail = otherQueue.m_Tail;
		m_Capacity = otherQueue.m_Capacity;
		m_Length = otherQueue.m_Length;

		otherQueue.m_Head = 0;
		otherQueue.m_Tail = 0;
		otherQueue.m_Length = 0;

		return *this;
	}

	template <class T>
	const T& Queue<T>::Peek() const
	{
		Assert(m_Length > 0);
		return m_Memory[m_Head];
	}

	template <class T>
	T Queue<T>::Pop()
	{
		Assert(m_Length > 0);
		--m_Length;
		return std::move(m_Memory[m_Head++]);
	}

	template <class T>
	void Queue<T>::Push(const T& element)
	{
		Assert(m_Length < m_Capacity);

		m_Memory[m_Tail] = element;
		++m_Length;
		m_Tail = (m_Tail + 1) % m_Capacity;
	}

	template <class T>
	void Queue<T>::Push(T&& element)
	{
		Assert(m_Length < m_Capacity);

		m_Memory[m_Tail] = std::move(element);
		++m_Length;
		m_Tail = (m_Tail + 1) % m_Capacity;
	}

	template <class T>
	const ArraySize& Queue<T>::GetCapacity() const
	{
		return m_Capacity;
	}
	
	template <class T>
	const ArraySize& Queue<T>::GetLength() const
	{
		return m_Length;
	}
}