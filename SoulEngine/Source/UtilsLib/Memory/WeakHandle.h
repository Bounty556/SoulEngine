/*
The interface between the MemoryManager and the objects that are allocated
within it. The MemoryManager returns UniqueHandles which can then be used to
form weak pointers to memory.
@file WeakHandle.h
@author Jacob Peterson
@edited 4/12/21
*/

#pragma once

#include <UtilsLib/Memory/MemoryManager.h>
#include <UtilsLib/Memory/UniqueHandle.h>

namespace Soul
{
	/*
	This is to be used similarly to std::weak_ptr<T>.
	*/
	template <class T>
	class WeakHandle
	{
	public:
		WeakHandle();
		WeakHandle(Handle* handle);
		WeakHandle(const UniqueHandle<T>& otherHandle);
		WeakHandle(const WeakHandle& otherHandle);
		WeakHandle(WeakHandle&& otherHandle);

		WeakHandle<T>& operator=(const WeakHandle& otherHandle);
		WeakHandle<T>& operator=(WeakHandle&& otherHandle);

		T* operator->();
		T& operator*();
		T& operator[](ArraySize index);
		const T* operator->() const;
		const T& operator*() const;
		const T& operator[](ArraySize index) const;

		bool operator==(const WeakHandle& other) const;
		bool operator!=(const WeakHandle& other) const;

		/*
		Returns whether this WeakHandle is active and usable.

		@return Boolean containing the validity of this WeakHandle.
		*/
		bool IsValid() const;

		/*
		Removes the handle from the ownership of this WeakHandle, and returns
		a pointer to the Handle to be managed elsewhere.

		@return Pointer to the Handle this WeakHandle used to own.
		*/
		Handle* Detach();

		/*
		USE CAUTIOUSLY!!! Gets the memory pointed to by this handle.

		@return Pointer to the memory managed by this handle.
		*/
		T* GetMemory();

	private:
		Handle* m_Handle; // Pointer to this WeakHandle's Handle.
		bool m_IsValid; // Whether this WeakHandle is active and usable.
	};

	template <class T>
	WeakHandle<T>::WeakHandle() :
		m_Handle(nullptr),
		m_IsValid(false)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(const WeakHandle& otherHandle) :
		m_Handle(otherHandle.m_Handle),
		m_IsValid(otherHandle.m_IsValid)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(Handle* handle) :
		m_Handle(handle),
		m_IsValid(true)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(const UniqueHandle<T>& otherHandle) :
		m_Handle(otherHandle.m_Handle),
		m_IsValid(otherHandle.m_IsValid)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(WeakHandle&& otherHandle) :
		m_Handle(otherHandle.m_Handle),
		m_IsValid(otherHandle.m_IsValid)
	{
		otherHandle.m_Handle = nullptr;
		otherHandle.m_IsValid = false;
	}

	template <class T>
	WeakHandle<T>& WeakHandle<T>::operator=(const WeakHandle& otherHandle)
	{
		m_Handle = otherHandle.m_Handle;
		m_IsValid = otherHandle.m_IsValid;

		return *this;
	}

	template <class T>
	WeakHandle<T>& WeakHandle<T>::operator=(WeakHandle&& otherHandle)
	{
		m_Handle = otherHandle.m_Handle;
		m_IsValid = otherHandle.m_IsValid;
		otherHandle.m_Handle = nullptr;
		otherHandle.m_IsValid = false;

		return *this;
	}

	template <class T>
	T* WeakHandle<T>::operator->()
	{
		return (T*)m_Handle->location;
	}

	template <class T>
	T& WeakHandle<T>::operator*()
	{
		return *((T*)(m_Handle->location));
	}

	template <class T>
	T& WeakHandle<T>::operator[](ArraySize index)
	{
		return ((T*)m_Handle->location)[index];
	}

	template <class T>
	const T* WeakHandle<T>::operator->() const
	{
		return (T*)m_Handle->location;
	}

	template <class T>
	const T& WeakHandle<T>::operator*() const
	{
		return *((T*)(m_Handle->location));
	}

	template <class T>
	const T& WeakHandle<T>::operator[](ArraySize index) const
	{
		return ((T*)m_Handle->location)[index];
	}

	template <class T>
	bool WeakHandle<T>::operator==(const WeakHandle& other) const
	{
		return m_Handle == other.m_Handle;
	}

	template <class T>
	bool WeakHandle<T>::operator!=(const WeakHandle& other) const
	{
		return m_Handle != other.m_Handle;
	}

	template <class T>
	bool WeakHandle<T>::IsValid() const
	{
		return m_IsValid;
	}

	template <class T>
	Handle* WeakHandle<T>::Detach()
	{
		Handle* handle = m_Handle;
		m_Handle = nullptr;
		m_IsValid = false;

		return handle;
	}

	template <class T>
	T* WeakHandle<T>::GetMemory()
	{
		return (T*)m_Handle->location;
	}
}