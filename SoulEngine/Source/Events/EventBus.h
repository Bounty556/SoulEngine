/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 12/23/2020
*/

#pragma once

#include <Events/EventTypes.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Containers/Queue.h>
#include <UtilsLib/Memory/MemoryManager.h>

namespace Soul
{
	struct Event
	{
		Events eEventType;
		Handle& hData;
	};

	class EventBus
	{
	public:

		/*
		Initializes this EventBus with the desired number of potential events.
		
		@param uiEventCount - How many events can be stored in this bus.
		*/
		static void StartUp(ArraySize uiEventCount);

		/*
		Shuts down and cleans up all necessary parts of this EventBus
		*/
		static void Shutdown();

		/*
		Adds a new event to the end of event queue to be dispatched.
		*/
		template <class T>
		static void QueueEvent(Events eEventType, UniqueHandle<T> hData);

		/*
		Dispatches the first N number of events to all attached event listeners.
		
		@param uiEventCount - Number of events to dispatch.
		*/
		static void DispatchEvents(UInt8 uiEventCount);

		EventBus() = delete;

	private:
		static UniqueHandle<Queue<Event>> _shEventQueue; // Queue of events to be dispatched.
		static bool _bIsSetup; // Whether this EventBus has been initialized.
	};

	template <class T>
	static void EventBus::QueueEvent(Events eEventType, UniqueHandle<T> hData)
	{
		Handle* hpData = hData.Detach();
		Event oEvent(eEventType, *hpData);
		_shEventQueue->Push(oEvent);
	}
}