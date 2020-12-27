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

		EventCallback fnCallback = [](Handle* hData){
			UniqueHandle<UInt8> hPrimitive(hData);
			Assert(*hPrimitive == 12);
		};
		oEventListener.RegisterCallback(Events::EventTest, fnCallback);

		UniqueHandle<UInt8> hInt = MemoryManager::Allocate<UInt8>(12);
		oEventListener.QueueEvent(Events::EventTest, std::move(hInt));

		return true;
	}

	bool EventTests::ObjectDataEvent()
	{
		EventListener oEventListener;

		TestClass oFake = { 1, 'a', 3.5f };

		EventCallback fnCallback = [oFake](Handle* hData) {
			UniqueHandle<TestClass> hObject(hData);
			Assert(*hObject == oFake);
		};
		oEventListener.RegisterCallback(Events::EventTest, fnCallback);

		UniqueHandle<TestClass> hObject = MemoryManager::Allocate<TestClass>(oFake);
		oEventListener.QueueEvent(Events::EventTest, std::move(hObject));

		return true;
	}

	bool EventTests::SeveralEvents()
	{
		EventListener oEventListener;

		EventCallback fnCallback = [](Handle* hData) {
			UniqueHandle<UInt8> hInt(hData);
			Assert(*hInt == 12);
		};
		EventCallback fnCallback2 = [](Handle* hData) {
			UniqueHandle<UInt8> hInt(hData);
			Assert(*hInt == 12);
		};

		oEventListener.RegisterCallback(Events::EventTest, fnCallback);

		UniqueHandle<UInt8> hInt = MemoryManager::Allocate<UInt8>(12);
		oEventListener.QueueEvent(Events::EventTest, std::move(hInt));

		return true;
	}
}