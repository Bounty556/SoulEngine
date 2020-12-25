/*
The interface between the MemoryManager and the objects that are allocated
within it. The MemoryManager returns UniqueHandles which can then be used to
form weak pointers to memory.
@file WeakHandle.h
@author Jacob Peterson
@edited 12/24/2020
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
		WeakHandle(Handle* oHandle);
		WeakHandle(const UniqueHandle<T>& oOtherHandle);
		WeakHandle(const WeakHandle& oOtherHandle);
		WeakHandle(WeakHandle&& oOtherHandle);

		WeakHandle<T>& operator=(const WeakHandle& oOtherHandle);
		WeakHandle<T>& operator=(WeakHandle&& oOtherHandle);

		T* operator->();
		T& operator*();
		T& operator[](ArraySize uiIndex);
		const T* operator->() const;
		const T& operator*() const;
		const T& operator[](ArraySize uiIndex) const;

		bool operator==(const WeakHandle& oOther) const;
		bool operator!=(const WeakHandle& oOther) const;

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
		Handle* _hpHandle; // Pointer to this WeakHandle's Handle.
		bool _bIsValid; // Whether this WeakHandle is active and usable.
	};

	template <class T>
	WeakHandle<T>::WeakHandle() :
		_hpHandle(nullptr),
		_bIsValid(false)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(const WeakHandle& oOtherHandle) :
		_hpHandle(oOtherHandle._hpHandle),
		_bIsValid(oOtherHandle._bIsValid)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(Handle* oHandle) :
		_hpHandle(oHandle),
		_bIsValid(true)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(const UniqueHandle<T>& oOtherHandle) :
		_hpHandle(oOtherHandle._hpHandle),
		_bIsValid(oOtherHandle._bIsValid)
	{

	}

	template <class T>
	WeakHandle<T>::WeakHandle(WeakHandle&& oOtherHandle) :
		_hpHandle(oOtherHandle._hpHandle),
		_bIsValid(oOtherHandle._bIsValid)
	{
		oOtherHandle._hpHandle = nullptr;
		oOtherHandle._bIsValid = false;
	}

	template <class T>
	WeakHandle<T>& WeakHandle<T>::operator=(const WeakHandle& oOtherHandle)
	{
		_hpHandle = oOtherHandle._hpHandle;
		_bIsValid = oOtherHandle._bIsValid;

		return *this;
	}

	template <class T>
	WeakHandle<T>& WeakHandle<T>::operator=(WeakHandle&& oOtherHandle)
	{
		_hpHandle = oOtherHandle._hpHandle;
		_bIsValid = oOtherHandle._bIsValid;
		oOtherHandle._hpHandle = nullptr;
		oOtherHandle._bIsValid = false;

		return *this;
	}

	template <class T>
	T* WeakHandle<T>::operator->()
	{
		return (T*)_hpHandle->pLocation;
	}

	template <class T>
	T& WeakHandle<T>::operator*()
	{
		return *((T*)(_hpHandle->pLocation));
	}

	template <class T>
	T& WeakHandle<T>::operator[](ArraySize uiIndex)
	{
		return ((T*)_hpHandle->pLocation)[uiIndex];
	}

	template <class T>
	const T* WeakHandle<T>::operator->() const
	{
		return (T*)_hpHandle->pLocation;
	}

	template <class T>
	const T& WeakHandle<T>::operator*() const
	{
		return *((T*)(_hpHandle->pLocation));
	}

	template <class T>
	const T& WeakHandle<T>::operator[](ArraySize uiIndex) const
	{
		return ((T*)_hpHandle->pLocation)[uiIndex];
	}

	template <class T>
	bool WeakHandle<T>::operator==(const WeakHandle& oOther) const
	{
		return _hpHandle == oOther._hpHandle;
	}

	template <class T>
	bool WeakHandle<T>::operator!=(const WeakHandle& oOther) const
	{
		return _hpHandle != oOther._hpHandle;
	}

	template <class T>
	bool WeakHandle<T>::IsValid() const
	{
		return _bIsValid;
	}

	template <class T>
	Handle* WeakHandle<T>::Detach()
	{
		Handle* opHandle = _hpHandle;
		_hpHandle = nullptr;
		_bIsValid = false;

		return opHandle;
	}

	template <class T>
	T* WeakHandle<T>::GetMemory()
	{
		return (T*)_hpHandle->pLocation;
	}
}