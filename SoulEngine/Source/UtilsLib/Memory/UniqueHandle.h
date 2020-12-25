/*
The interface between the MemoryManager and the objects that are allocated
within it. The MemoryManager returns UniqueHandles which can then be used like
normal pointers to memory.
@file UniqueHandle.h
@author Jacob Peterson
@edited 12/23/2020
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
		UniqueHandle(Handle* oHandle);
		UniqueHandle(UniqueHandle&& oOtherHandle);

		~UniqueHandle();

		UniqueHandle<T>& operator=(UniqueHandle&& oOtherHandle);
		
		T* operator->();
		T& operator*();
		T& operator[](ArraySize uiIndex);
		const T* operator->() const;
		const T& operator*() const;
		const T& operator[](ArraySize uiIndex) const;

		bool operator==(const UniqueHandle& oOther) const;
		bool operator!=(const UniqueHandle& oOther) const;

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
		Handle* _hpHandle; // Pointer to this UniqueHandle's Handle.
		bool _bIsValid; // Whether this UniqueHandle is active and usable.

		friend WeakHandle;
	};

	template <class T>
	UniqueHandle<T>::UniqueHandle() :
		_hpHandle(nullptr),
		_bIsValid(false)
	{

	}

	template <class T>
	UniqueHandle<T>::UniqueHandle(Handle* oHandle) :
		_hpHandle(oHandle),
		_bIsValid(true)
	{
	
	}

	template <class T>
	UniqueHandle<T>::UniqueHandle(UniqueHandle&& oOtherHandle) :
		_hpHandle(oOtherHandle._hpHandle),
		_bIsValid(oOtherHandle._bIsValid)
	{
		oOtherHandle._hpHandle = nullptr;
		oOtherHandle._bIsValid = false;
	}

	template <class T>
	UniqueHandle<T>::~UniqueHandle()
	{
		if (_bIsValid)
		{
			MemoryManager::Deallocate<T>(*_hpHandle);
		}
	}

	template <class T>
	UniqueHandle<T>& UniqueHandle<T>::operator=(UniqueHandle&& oOtherHandle)
	{
		if (_bIsValid)
		{
			MemoryManager::Deallocate<T>(*_hpHandle);
		}

		_hpHandle = oOtherHandle._hpHandle;
		_bIsValid = oOtherHandle._bIsValid;
		oOtherHandle._hpHandle = nullptr;
		oOtherHandle._bIsValid = false;

		return *this;
	}

	template <class T>
	T* UniqueHandle<T>::operator->()
	{
		return (T*)_hpHandle->pLocation;
	}

	template <class T>
	T& UniqueHandle<T>::operator*()
	{
		return *((T*)(_hpHandle->pLocation));
	}

	template <class T>
	T& UniqueHandle<T>::operator[](ArraySize uiIndex)
	{
		return ((T*)_hpHandle->pLocation)[uiIndex];
	}

	template <class T>
	const T* UniqueHandle<T>::operator->() const
	{
		return (T*)_hpHandle->pLocation;
	}

	template <class T>
	const T& UniqueHandle<T>::operator*() const
	{
		return *((T*)(_hpHandle->pLocation));
	}

	template <class T>
	const T& UniqueHandle<T>::operator[](ArraySize uiIndex) const
	{
		return ((T*)_hpHandle->pLocation)[uiIndex];
	}

	template <class T>
	bool UniqueHandle<T>::operator==(const UniqueHandle& oOther) const
	{
		return _hpHandle == oOther._hpHandle;
	}

	template <class T>
	bool UniqueHandle<T>::operator!=(const UniqueHandle& oOther) const
	{
		return _hpHandle != oOther._hpHandle;
	}

	template <class T>
	bool UniqueHandle<T>::IsValid() const
	{
		return _bIsValid;
	}

	template <class T>
	Handle* UniqueHandle<T>::Detach()
	{
		Handle* opHandle = _hpHandle;
		_hpHandle = nullptr;
		_bIsValid = false;

		return opHandle;
	}

	template <class T>
	void UniqueHandle<T>::Deallocate()
	{
		Assert(_bIsValid);

		MemoryManager::Deallocate<T>(*_hpHandle);
		_hpHandle = nullptr;
		_bIsValid = false;
	}

	template <class T>
	T* UniqueHandle<T>::GetMemory()
	{
		return (T*)_hpHandle->pLocation;
	}
}