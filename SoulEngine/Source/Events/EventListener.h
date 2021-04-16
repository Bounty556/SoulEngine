/*
Manages adding and removing callbacks with the EventBus to provide a thin
layer of abstraction and automation.
@file EventListener.h
@author Jacob Peterson
@edited 4/14/21
*/

#pragma once

#include <Events/EventBus.h>

namespace Soul
{
	struct RegisteredCallback
	{
		Events eventType; // The event this callback is registered for.
		CallbackId callbackId; // The id of the associated callback.
	};

	/*
	Used to add and remove (sometimes automatically) callbacks and queue Events
	to the EventBus. Callbacks can be managed with the RegisterCallback() and
	UnregisterCallback() functions. Events can be queued with the QueueEvent()
	function. Any callbacks registered when the EventListener is destroyed are
	automatically unregistered.
	*/
	class EventListener
	{
	public:
		EventListener();
		EventListener(EventListener&& otherListener);

		~EventListener();

		EventListener& operator=(EventListener&& otherListener);

		/*
		Adds a new event to the end of event queue to be dispatched.
		*/
		void QueueEvent(Events eventType, void* data);

		/*
		Adds a new callback to be called when the given event is triggered.

		@param eventType - The event to register the callback for.

		@param callback - The callback function to be registered.

		@return CallbackId containing the unique callback id.
		*/
		CallbackId RegisterCallback(Events eventType, EventCallback callback);

		/*
		Unregisters the callback with the given id from the given event.

		@param eventType - The event to unregister the callback from.
		*/
		void UnregisterCallback(Events eventType);

		EventListener(const EventListener&) = delete;
		EventListener& operator=(const EventListener&) = delete;

	private:
		Vector<RegisteredCallback> m_RegisteredCallbacks; // The callback Id for each registeredevent.
	};
}