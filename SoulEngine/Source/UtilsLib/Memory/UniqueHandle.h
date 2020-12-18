/*
The interface between the MemoryManager and the objects that are allocated
within it. The MemoryManager returns UniqueHandles which can then be used like
normal pointers to memory.
@file UniqueHandle.h
@author Jacob Peterson
@version 1 12/17/2020
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
		UniqueHandle(Handle& oHandle);
		UniqueHandle(UniqueHandle&& oOtherHandle);

		~UniqueHandle();

		UniqueHandle<T>& operator=(UniqueHandle&& oOtherHandle);
		T* operator->() const;
		T& operator*() const;

		UniqueHandle() = delete;
		UniqueHandle(const UniqueHandle&) = delete;
		UniqueHandle<T>& operator=(const UniqueHandle&) = delete;

	private:
		Handle* _opHandle; // Pointer to this UniqueHandle's Handle.
	};

	template <class T>
	UniqueHandle<T>::UniqueHandle(Handle& oHandle) :
		_opHandle(oHandle)
	{ }

	template <class T>
	UniqueHandle<T>::UniqueHandle(UniqueHandle&& oOtherHandle) :
		_opHandle(oOtherHandle._opHandle)
	{
		oOtherHandle._opHandle = nullptr;
	}

	template <class T>
	UniqueHandle<T>::~UniqueHandle()
	{
		if (_opHandle)
		{
			MemoryManager::Deallocate<T>(*_opHandle);
		}
	}

	template <class T>
	UniqueHandle<T>& UniqueHandle<T>::operator=(UniqueHandle&& oOtherHandle)
	{
		_opHandle = oOtherHandle._opHandle;
		oOtherHandle._opHandle = nullptr;
	}

	template <class T>
	T* UniqueHandle<T>::operator->() const
	{
		return (T*)_opHandle->uipLocation;
	}

	template <class T>
	T& UniqueHandle<T>::operator*() const
	{
		return *((T*)(_opHandle->uipLocation));
	}
}