/*
A self-resizing array that behaves similarly to the C Standard Library Vector.
@file Vector.h
@author Jacob Peterson
@edited 12/23/2020
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
		Vector(ArraySize uiCapacity);
		Vector(Vector&& oOtherVector);

		Vector<T>& operator=(Vector&& oOtherVector);
		T& operator[](ArraySize uiIndex);
		const T& operator[](ArraySize uiIndex) const;

		/*
		Adds a new element to the end of this Vector.

		@param oElement - The element to add to this Vector.
		*/
		void Push(const T& oElement);

		/*
		Adds a new element to the end of this Vector.

		@param oElement - The element to add to this Vector.
		*/
		void Push(T&& oElement);

		/*
		Removes and returns the element at the end of this Vector.

		@return Element at the end of the Vector.
		*/
		T Pop();

		/*
		Removes the element at the given index.
		
		@param uiIndex - The index of the element to be removed.
		*/
		void Remove(ArraySize uiIndex);

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
		UniqueHandle<T> _hElements;
		ArraySize _uiCapacity;
		ArraySize _uiLength;
	};

	template <class T>
	Vector<T>::Vector(ArraySize uiCapacity) :
		_hElements(MemoryManager::AllocateArray<T>(uiCapacity)),
		_uiCapacity(uiCapacity),
		_uiLength(0)
	{

	}

	template <class T>
	Vector<T>::Vector(Vector&& oOtherVector) :
		_hElements(std::move(oOtherVector._hElements)),
		_uiCapacity(oOtherVector._uiCapacity),
		_uiLength(oOtherVector._uiLength)
	{
		oOtherVector._uiCapacity = 0;
		oOtherVector._uiLength = 0;
	}

	template <class T>
	Vector<T>& Vector<T>::operator=(Vector&& oOtherVector)
	{
		_hElements = std::move(oOtherVector._hElements);
		_uiCapacity = oOtherVector._uiCapacity;
		_uiLength = oOtherVector._uiLength;
		oOtherVector._uiCapacity = 0;
		oOtherVector._uiLength = 0;

		return *this;
	}

	template <class T>
	T& Vector<T>::operator[](ArraySize uiIndex)
	{
		Assert(uiIndex > 0);
		Assert(uiIndex < _uiLength);
		return _hElements[uiIndex];
	}

	template <class T>
	const T& Vector<T>::operator[](ArraySize uiIndex) const
	{
		Assert(uiIndex > 0);
		Assert(uiIndex < _uiLength);
		return _hElements[uiIndex];
	}

	template <class T>
	void Vector<T>::Push(const T& oElement)
	{
		if (_uiLength == _uiCapacity)
		{
			Resize();
		}

		_hElements[_uiLength++] = oElement;
	}

	template <class T>
	void Vector<T>::Push(T&& oElement)
	{
		if (_uiLength == _uiCapacity)
		{
			Resize();
		}

		_hElements[_uiLength++] = std::move(oElement);
	}

	template <class T>
	T Vector<T>::Pop()
	{
		Assert(_uiLength > 0);
		
		return std::move(_hElements[--_uiLength]);
	}

	template <class T>
	void Vector<T>::Remove(ArraySize uiIndex)
	{
		Assert(uiIndex > 0);
		Assert(uiIndex < _uiLength);
		_hElements[uiIndex].~T();

		/*
		Move memory in front of this element over.
		*/
		ArraySize uiElementsToMove = _uiLength - (uiIndex + 1);
		memcpy(&(_hElements[uiIndex]), &(_hElements[uiIndex + 1]),
			uiElementsToMove * sizeof(T));
		--_uiLength;
	}

	template <class T>
	ArraySize Vector<T>::Length() const
	{
		return _uiLength;
	}

	template <class T>
	void Vector<T>::Resize()
	{
		/*
		Request new memory for expanded capacity.
		*/
		ArraySize uiOldCapacity = _uiCapacity;
		_uiCapacity = _uiCapacity * 2 + 1;
		UniqueHandle<T> hNewMemory = MemoryManager::AllocateArray<T>(_uiCapacity);

		/*
		Move memory and deallocate old memory.
		*/
		memcpy(hNewMemory.GetMemory(), _hElements.GetMemory(),
			sizeof(T) * uiOldCapacity);
		_hElements = std::move(hNewMemory);
	}
}