/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 12/24/2020
*/

#pragma once

#include <functional>

#include <Events/EventTypes.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Containers/Queue.h>
#include <UtilsLib/Containers/Vector.h>
#include <UtilsLib/Memory/MemoryManager.h>

typedef std::function<void(Soul::Handle*)> EventCallback;
typedef UInt64 CallbackId;

namespace Soul
{
	struct Event
	{
		Events eEventType; // This Event's type
		Handle* hData; // The data associated with this event, if any.
	};

	struct Callback
	{
		EventCallback fnCallback; // The actual callback.
		CallbackId uiCallbackId; // The id of this callback.
	};

	/*
	Singleton Event system which manages Events, registering callbacks for
	events, and dispatching the events to call those callbacks. This system
	needs to have been initialized with StartUp() before being used.

	The functions for managing callbacks, such as RegisterCallback() and
	UnregisterCallback() should only be used through the EventListener class.
	*/
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

		/*
		Adds a new callback to be called when the given event is triggered.

		@param eEventType - The event to register the callback for.

		@param fnCallback - The callback function to be registered.

		@return CallbackId containing the unique callback id.
		*/
		static CallbackId RegisterCallback(Events eEventType, EventCallback fnCallback);

		/*
		Unregisters the callback with the given id from the given event.

		@param eEventType - The event to unregister the callback from.

		@param uiId - The id of the callback to unregister.
		*/
		static void UnregisterCallback(Events eEventType, CallbackId uiId);

		EventBus() = delete;

	private:
		static UniqueHandle<Queue<Event>> _shEventQueue; // Queue of events to be dispatched.
		static UniqueHandle<Vector<Callback>> _shRegisteredCallbacks; // All registered callbacks
		static CallbackId _uiCallbackCount; // Used for identifying unique callbacks.
		static bool _bIsSetup; // Whether this EventBus has been initialized.
	};

	template <class T>
	static void EventBus::QueueEvent(Events eEventType, UniqueHandle<T> hData)
	{
		Event oEvent{ eEventType, hData.Detach() };
		_shEventQueue->Push(oEvent);
	}
}