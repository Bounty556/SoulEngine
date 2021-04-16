/*
Tests for the EventBus and EventListener classes.
@file EventTests.cpp
@author Jacob Peterson
@edited 4/15/21
*/

#include "EventTests.h"

#include <TestsLib/TestMacros.h>
#include <TestsLib/TestClass.h>
#include <Events/EventBus.h>
#include <Events/EventListener.h>
#include <UtilsLib/Logger.h>

namespace Soul
{
	void EventTests::RunAllTests()
	{
		RunTest(PrimitiveDataEvent);
		RunTest(ObjectDataEvent);
		RunTest(ExpandedCallbackVector);
		RunTest(SeveralEvents);
	}

	bool EventTests::PrimitiveDataEvent()
	{
		EventListener eventListener;

		EventCallback callback = [](void* data){
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};
		eventListener.RegisterCallback(Events::EventTest, callback);

		UInt8* intPointer = MemoryManager::AllocateVolatile<UInt8>();
		*intPointer = 12;
		eventListener.QueueEvent(Events::EventTest, intPointer);

		EventBus::DispatchEvents();

		return true;
	}

	bool EventTests::ObjectDataEvent()
	{
		EventListener eventListener;

		TestClass fakeClass = { 1, 'a', 3.5f };

		EventCallback callback = [fakeClass](void* data) {
			TestClass* objectPointer = (TestClass*)data;
			Assert(*objectPointer == fakeClass);
		};
		eventListener.RegisterCallback(Events::EventTest, callback);

		TestClass* object = MemoryManager::AllocateVolatile<TestClass>();
		*object = fakeClass;
		eventListener.QueueEvent(Events::EventTest, object);

		EventBus::DispatchEvents();

		return true;
	}

	bool EventTests::ExpandedCallbackVector()
	{
		UniqueHandle<Vector<Callback>> registeredCallbacks =
			MemoryManager::AllocateArray<Vector<Callback>>((ArraySize)Events::EventTotal);

		for (ArraySize i = 0; i < (ArraySize)Events::EventTotal; ++i)
		{
			Vector<Callback> vec(4);
			registeredCallbacks[i] = std::move(vec);
		}

		Callback callback1{ [](void* data) { }, 0 };
		Callback callback2{ [](void* data) { }, 1 };
		Callback callback3{ [](void* data) { }, 2 };
		Callback callback4{ [](void* data) { }, 3 };
		Callback callback5{ [](void* data) { }, 4 };
		Callback callback6{ [](void* data) { }, 5 };
		Callback callback7{ [](void* data) { }, 6 };

		registeredCallbacks[0].Push(callback1);
		registeredCallbacks[0].Push(callback2);
		registeredCallbacks[0].Push(callback3);
		registeredCallbacks[0].Push(callback4);
		registeredCallbacks[0].Push(callback5);
		registeredCallbacks[0].Push(callback6);
		registeredCallbacks[0].Push(callback7);

		return true;
	}

	bool EventTests::SeveralEvents()
	{
		/*EventListener eventListener;

		EventCallback callback = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};

		EventCallback callback2 = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};
		
		EventCallback callback3 = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};

		EventCallback callback4 = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};

		EventCallback callback5 = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};

		EventCallback callback6 = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};

		EventCallback callback7 = [](void* data) {
			UInt8* intPointer = (UInt8*)data;
			Assert(*intPointer == 12);
		};

		eventListener.RegisterCallback(Events::EventTest, callback);
		eventListener.RegisterCallback(Events::EventTest, callback2);
		eventListener.RegisterCallback(Events::EventTest, callback3);
		eventListener.RegisterCallback(Events::EventTest, callback4);
		eventListener.RegisterCallback(Events::EventTest, callback5);
		eventListener.RegisterCallback(Events::EventTest, callback6);
		eventListener.RegisterCallback(Events::EventTest, callback7);

		UInt8* intPointer = MemoryManager::AllocateVolatile<UInt8>();
		*intPointer = 12;
		eventListener.QueueEvent(Events::EventTest, intPointer);

		EventBus::DispatchEvents();*/

		return true;
	}
}