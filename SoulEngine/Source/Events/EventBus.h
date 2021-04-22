/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 4/21/21
*/

#pragma once

#include <functional>

#include <Events/EventTypes.h>
#include <Memory/MemoryManager.h>
#include <UtilsLib/CommonTypes.h>
#include <UtilsLib/Containers/Queue.h>
#include <UtilsLib/Containers/Vector.h>

typedef std::function<void(void*)> EventCallback;
typedef UInt64 CallbackId;

namespace Soul
{
	struct Event
	{
		Events eventType; // This Event's type
		void* pointerToData; // The data associated with this event, if any.
	};

	struct Callback
	{
		EventCallback callbackFunction; // The actual callback.
		CallbackId callbackId; // The id of this callback.
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
		
		@param eventCount - How many events can be stored in this bus.
		*/
		static void StartUp(ArraySize eventCount);

		/*
		Shuts down and cleans up all necessary parts of this EventBus
		*/
		static void Shutdown();

		/*
		Adds a new event to the end of event queue to be dispatched.
		*/
		static void QueueEvent(Events eventType, void* data);

		/*
		Dispatches all events to attached event listeners.
		*/
		static void DispatchEvents();

		/*
		Adds a new callback to be called when the given event is triggered.

		@param eventType - The event to register the callback for.

		@param callback - The callback function to be registered.

		@return callbackId containing the unique callback id.
		*/
		static CallbackId RegisterCallback(Events eventType, EventCallback callback);

		/*
		Unregisters the callback with the given id from the given event.

		@param eventType - The event to unregister the callback from.

		@param callbackId - The id of the callback to unregister.
		*/
		static void UnregisterCallback(Events eventType, CallbackId callbackId);

		EventBus() = delete;

	private:
		static UniqueHandle<Queue<Event>> m_EventQueue; // Queue of events to be dispatched.
		static UniqueHandle<Vector<Callback>> m_RegisteredCallbacks; // All registered callbacks
		static CallbackId m_CallbackCount; // Used for identifying unique callbacks.
		static bool m_IsSetup; // Whether this EventBus has been initialized.
	};
}