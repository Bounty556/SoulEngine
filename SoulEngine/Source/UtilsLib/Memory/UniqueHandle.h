/*
The interface between the MemoryManager and the objects that are allocated
within it. The MemoryManager returns UniqueHandles which can then be used like
normal pointers to memory.
@file UniqueHandle.h
@author Jacob Peterson
@edited 4/12/2020
*/

#pragma once

#include <UtilsLib/Memory/MemoryManager.h>

namespace Soul
{
	template <class T>
	class WeakHandle;

	/*
	This is to be used similarly to std::unique_ptr<T>. 
	*/
	template <class T>
	class UniqueHandle
	{
	public:
		UniqueHandle();
		UniqueHandle(Handle* handle);
		UniqueHandle(UniqueHandle&& otherHandle);

		~UniqueHandle();

		UniqueHandle<T>& operator=(UniqueHandle&& otherHandle);
		
		T* operator->();
		T& operator*();
		T& operator[](ArraySize index);
		const T* operator->() const;
		const T& operator*() const;
		const T& operator[](ArraySize index) const;

		bool operator==(const UniqueHandle& other) const;
		bool operator!=(const UniqueHandle& other) const;

		/*
		Returns whether this UniqueHandle is active and usable.

		@return Boolean containing the validity of this UniqueHandle.
		*/
		bool IsValid() const;

		/*
		Removes the handle from the ownership of this UniqueHandle, and returns
		a pointer to the Handle to be managed elsewhere.

		@return Pointer to the Handle this UniqueHandle used to own.
		*/
		Handle* Detach();

		/*
		Deallocates the memory pointed to by the underlying handle, and makes
		this UniqueHandle invalid.
		*/
		void Deallocate();

		/*
		USE CAUTIOUSLY!!! Gets the memory at pointed to by this handle.

		@return Pointer to the memory managed by this handle.
		*/
		T* GetMemory();

		UniqueHandle(const UniqueHandle&) = delete;
		UniqueHandle<T>& operator=(const UniqueHandle&) = delete;

	private:
		Handle* m_Handle; // Pointer to this UniqueHandle's Handle.
		bool m_IsValid; // Whether this UniqueHandle is active and usable.

		friend WeakHandle;
	};

	template <class T>
	UniqueHandle<T>::UniqueHandle() :
		m_Handle(nullptr),
		m_IsValid(false)
	{

	}

	template <class T>
	UniqueHandle<T>::UniqueHandle(Handle* handle) :
		m_Handle(handle),
		m_IsValid(true)
	{
	
	}

	template <class T>
	UniqueHandle<T>::UniqueHandle(UniqueHandle&& otherHandle) :
		m_Handle(otherHandle.m_Handle),
		m_IsValid(otherHandle.m_IsValid)
	{
		otherHandle.m_Handle = nullptr;
		otherHandle.m_IsValid = false;
	}

	template <class T>
	UniqueHandle<T>::~UniqueHandle()
	{
		if (m_IsValid)
		{
			MemoryManager::Deallocate<T>(*m_Handle);
		}
	}

	template <class T>
	UniqueHandle<T>& UniqueHandle<T>::operator=(UniqueHandle&& otherHandle)
	{
		if (m_IsValid)
		{
			MemoryManager::Deallocate<T>(*m_Handle);
		}

		m_Handle = otherHandle.m_Handle;
		m_IsValid = otherHandle.m_IsValid;
		otherHandle.m_Handle = nullptr;
		otherHandle.m_IsValid = false;

		return *this;
	}

	template <class T>
	T* UniqueHandle<T>::operator->()
	{
		return (T*)m_Handle->location;
	}

	template <class T>
	T& UniqueHandle<T>::operator*()
	{
		return *((T*)(m_Handle->location));
	}

	template <class T>
	T& UniqueHandle<T>::operator[](ArraySize index)
	{
		return ((T*)m_Handle->location)[index];
	}

	template <class T>
	const T* UniqueHandle<T>::operator->() const
	{
		return (T*)m_Handle->location;
	}

	template <class T>
	const T& UniqueHandle<T>::operator*() const
	{
		return *((T*)(m_Handle->location));
	}

	template <class T>
	const T& UniqueHandle<T>::operator[](ArraySize index) const
	{
		return ((T*)m_Handle->location)[index];
	}

	template <class T>
	bool UniqueHandle<T>::operator==(const UniqueHandle& other) const
	{
		return m_Handle == other.m_Handle;
	}

	template <class T>
	bool UniqueHandle<T>::operator!=(const UniqueHandle& other) const
	{
		return m_Handle != other.m_Handle;
	}

	template <class T>
	bool UniqueHandle<T>::IsValid() const
	{
		return m_IsValid;
	}

	template <class T>
	Handle* UniqueHandle<T>::Detach()
	{
		Handle* handle = m_Handle;
		m_Handle = nullptr;
		m_IsValid = false;

		return handle;
	}

	template <class T>
	void UniqueHandle<T>::Deallocate()
	{
		Assert(m_IsValid);

		MemoryManager::Deallocate<T>(*m_Handle);
		m_Handle = nullptr;
		m_IsValid = false;
	}

	template <class T>
	T* UniqueHandle<T>::GetMemory()
	{
		return (T*)m_Handle->location;
	}
}