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
	/*
	This is to be used similarly to std::unique_ptr<T>. 
	*/
	template <class T>
	class UniqueHandle
	{
	public:
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

		UniqueHandle() = delete;
		UniqueHandle(const UniqueHandle&) = delete;
		UniqueHandle<T>& operator=(const UniqueHandle&) = delete;

	private:
		Handle* _opHandle; // Pointer to this UniqueHandle's Handle.
		bool _bIsValid; // Whether this UniqueHandle is active and usable.
	};

	template <class T>
	UniqueHandle<T>::UniqueHandle(Handle* oHandle) :
		_opHandle(oHandle),
		_bIsValid(true)
	{ }

	template <class T>
	UniqueHandle<T>::UniqueHandle(UniqueHandle&& oOtherHandle) :
		_opHandle(oOtherHandle._opHandle),
		_bIsValid(oOtherHandle._bIsValid)
	{
		oOtherHandle._opHandle = nullptr;
		oOtherHandle._bIsValid = false;
	}

	template <class T>
	UniqueHandle<T>::~UniqueHandle()
	{
		if (_bIsValid)
		{
			MemoryManager::Deallocate<T>(*_opHandle);
		}
	}

	template <class T>
	UniqueHandle<T>& UniqueHandle<T>::operator=(UniqueHandle&& oOtherHandle)
	{
		if (_bIsValid)
		{
			MemoryManager::Deallocate<T>(*_opHandle);
		}

		_opHandle = oOtherHandle._opHandle;
		_bIsValid = oOtherHandle._bIsValid;
		oOtherHandle._opHandle = nullptr;
		oOtherHandle._bIsValid = false;

		return *this;
	}

	template <class T>
	T* UniqueHandle<T>::operator->()
	{
		return (T*)_opHandle->pLocation;
	}

	template <class T>
	T& UniqueHandle<T>::operator*()
	{
		return *((T*)(_opHandle->pLocation));
	}

	template <class T>
	T& UniqueHandle<T>::operator[](ArraySize uiIndex)
	{
		return ((T*)_opHandle->pLocation)[uiIndex];
	}

	template <class T>
	const T* UniqueHandle<T>::operator->() const
	{
		return (T*)_opHandle->pLocation;
	}

	template <class T>
	const T& UniqueHandle<T>::operator*() const
	{
		return *((T*)(_opHandle->pLocation));
	}

	template <class T>
	const T& UniqueHandle<T>::operator[](ArraySize uiIndex) const
	{
		return ((T*)_opHandle->pLocation)[uiIndex];
	}

	template <class T>
	bool UniqueHandle<T>::operator==(const UniqueHandle& oOther) const
	{
		return _opHandle == oOther._opHandle;
	}

	template <class T>
	bool UniqueHandle<T>::operator!=(const UniqueHandle& oOther) const
	{
		return _opHandle != oOther._opHandle;
	}

	template <class T>
	bool UniqueHandle<T>::IsValid() const
	{
		return _bIsValid;
	}

	template <class T>
	Handle* UniqueHandle<T>::Detach()
	{
		Handle* opHandle = _opHandle;
		_opHandle = nullptr;
		_bIsValid = false;

		return opHandle;
	}

	template <class T>
	void UniqueHandle<T>::Deallocate()
	{
		Assert(_bIsValid);

		MemoryManager::Deallocate<T>(*_opHandle);
		_opHandle = nullptr;
	}
}