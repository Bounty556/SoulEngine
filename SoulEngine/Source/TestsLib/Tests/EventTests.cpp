/*
Tests for the EventBus and EventListener classes.
@file EventTests.cpp
@author Jacob Peterson
@edited 12/26/2020
*/

#include "EventTests.h"

#include <TestsLib/TestMacros.h>
#include <TestsLib/TestClass.h>
#include <Events/EventBus.h>
#include <Events/EventListener.h>

namespace Soul
{
	void EventTests::RunAllTests()
	{
		RunTest(PrimitiveDataEvent);
		RunTest(ObjectDataEvent);
		RunTest(SeveralEvents);
	}

	bool EventTests::PrimitiveDataEvent()
	{
		EventListener oEventListener;

		EventCallback fnCallback = [](void* pData){
			UInt8* pPrimitive = (UInt8*)pData;
			Assert(*pPrimitive == 12);
		};
		oEventListener.RegisterCallback(Events::EventTest, fnCallback);

		UInt8* pInt = MemoryManager::AllocateVolatile<UInt8>();
		*pInt = 12;
		oEventListener.QueueEvent(Events::EventTest, pInt);

		return true;
	}

	bool EventTests::ObjectDataEvent()
	{
		EventListener oEventListener;

		TestClass oFake = { 1, 'a', 3.5f };

		EventCallback fnCallback = [oFake](void* pData) {
			TestClass* pObject = (TestClass*)pData;
			Assert(*pObject == oFake);
		};
		oEventListener.RegisterCallback(Events::EventTest, fnCallback);

		TestClass* pObject = MemoryManager::AllocateVolatile<TestClass>();
		*pObject = oFake;
		oEventListener.QueueEvent(Events::EventTest, pObject);

		return true;
	}

	bool EventTests::SeveralEvents()
	{
		EventListener oEventListener;

		EventCallback fnCallback = [](void* pData) {
			UInt8* pInt = (UInt8*)pData;
			Assert(*pInt == 12);
		};
		EventCallback fnCallback2 = [](void* pData) {
			UInt8* pInt = (UInt8*)pData;
			Assert(*pInt == 12);
		};

		oEventListener.RegisterCallback(Events::EventTest, fnCallback);

		UInt8* pInt = MemoryManager::AllocateVolatile<UInt8>();
		*pInt = 12;
		oEventListener.QueueEvent(Events::EventTest, pInt);

		return true;
	}
}