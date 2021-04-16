/*
A self-resizing array that behaves similarly to the C Standard Library Vector.
@file Vector.h
@author Jacob Peterson
@edited 4/15/21
*/

#pragma once

#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	/*
	A self-resizing array that behaves similarly to the C Standard Library
	Vector.
	*/
	template <class T>
	class Vector
	{
	public:
		Vector(ArraySize capacity);
		Vector(Vector&& otherVector);

		Vector<T>& operator=(Vector&& otherVector);
		T& operator[](ArraySize index);
		const T& operator[](ArraySize index) const;

		/*
		Adds a new element to the end of this Vector.

		@param element - The element to add to this Vector.
		*/
		void Push(const T& element);

		/*
		Adds a new element to the end of this Vector.

		@param element - The element to add to this Vector.
		*/
		void Push(T&& element);

		/*
		Removes and returns the element at the end of this Vector.

		@return Element at the end of the Vector.
		*/
		T Pop();

		/*
		Removes the element at the given index.
		
		@param index - The index of the element to be removed.
		*/
		void Remove(ArraySize index);

		/*
		Gets the current number of elements in this Vector.

		@return ArraySize containing the number of elements in this Vector.
		*/
		ArraySize Length() const;

		Vector() = delete;
		Vector(const Vector&) = delete;

	private:
		/*
		Doubles the capacity of this Vector.
		*/
		void Resize();

	private:
		UniqueHandle<T> m_Elements;
		ArraySize m_Capacity;
		ArraySize m_Length;
	};

	template <class T>
	Vector<T>::Vector(ArraySize capacity) :
		m_Elements(MemoryManager::AllocateArray<T>(capacity)),
		m_Capacity(capacity),
		m_Length(0)
	{

	}

	template <class T>
	Vector<T>::Vector(Vector&& otherVector) :
		m_Elements(std::move(otherVector.m_Elements)),
		m_Capacity(otherVector.m_Capacity),
		m_Length(otherVector.m_Length)
	{
		otherVector.m_Capacity = 0;
		otherVector.m_Length = 0;
	}

	template <class T>
	Vector<T>& Vector<T>::operator=(Vector&& otherVector)
	{
		m_Elements = std::move(otherVector.m_Elements);
		m_Capacity = otherVector.m_Capacity;
		m_Length = otherVector.m_Length;
		otherVector.m_Capacity = 0;
		otherVector.m_Length = 0;

		return *this;
	}

	template <class T>
	T& Vector<T>::operator[](ArraySize index)
	{
		Assert(index >= 0);
		Assert(index < m_Length);
		return m_Elements[index];
	}

	template <class T>
	const T& Vector<T>::operator[](ArraySize index) const
	{
		Assert(index >= 0);
		Assert(index < m_Length);
		return m_Elements[index];
	}

	template <class T>
	void Vector<T>::Push(const T& element)
	{
		if (m_Length == m_Capacity)
		{
			Resize();
		}

		m_Elements[m_Length++] = element;
	}

	template <class T>
	void Vector<T>::Push(T&& element)
	{
		if (m_Length == m_Capacity)
		{
			Resize();
		}

		m_Elements[m_Length++] = std::move(element);
	}

	template <class T>
	T Vector<T>::Pop()
	{
		Assert(m_Length > 0);
		
		return std::move(m_Elements[--m_Length]);
	}

	template <class T>
	void Vector<T>::Remove(ArraySize index)
	{
		Assert(index >= 0);
		Assert(index < m_Length);
		m_Elements[index].~T();

		// Move memory in front of this element over.
		// New method: Move every element over individually, clean up old memory

		ArraySize elementsToMove = m_Length - (index + 1);

		for (ArraySize i = 0; i < m_Length - 1; ++i)
		{
			m_Elements[index] = std::move(m_Elements[index + 1]);
		}

		--m_Length;
		memset(&(m_Elements[m_Length]), 0, sizeof(T));

		// The following doesn't work. I assume this is because it's only doing a shallow
		// memory copy, so when we actually try to do anything the memory we could get a bunch
		// of errors because it's trying to reference deep data that couldn't be transferred

		//memcpy(&(m_Elements[index]), &(m_Elements[index + 1]),
		//	elementsToMove * sizeof(T));
		//--m_Length;
		//memset(&(m_Elements[m_Length]), 0, sizeof(T));
	}

	template <class T>
	ArraySize Vector<T>::Length() const
	{
		return m_Length;
	}

	template <class T>
	void Vector<T>::Resize()
	{
		/*
		Request new memory for expanded capacity.
		*/
		ArraySize oldCapacity = m_Capacity;
		m_Capacity = m_Capacity * 2 + 1;
		UniqueHandle<T> newMemory = MemoryManager::AllocateArray<T>(m_Capacity);
		
		// New method: Move every element over individually, clean up old memory

		for (ArraySize i = 0; i < oldCapacity; ++i)
		{
			newMemory[i] = std::move(m_Elements[i]);
		}
		memset(m_Elements.GetMemory(), 0, sizeof(T) * oldCapacity);
		m_Elements = std::move(newMemory);

		
		// The following doesn't work. I assume this is because it's only doing a shallow
		// memory copy, so when we actually try to do anything the memory we could get a bunch
		// of errors because it's trying to reference deep data that couldn't be transferred

		///*
		//Move memory and deallocate old memory.
		//*/
		//memcpy(newMemory.GetMemory(), m_Elements.GetMemory(),
		//	sizeof(T) * oldCapacity);
		//memset(m_Elements.GetMemory(), 0, sizeof(T) * oldCapacity);
		//m_Elements = std::move(newMemory);
	}
}