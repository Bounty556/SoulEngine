/*
Manages adding and removing callbacks with the EventBus to provide a thin
layer of abstraction and automation.
@file EventListener.h
@author Jacob Peterson
@edited 12/26/2020
*/

#pragma once

#include <Events/EventBus.h>

namespace Soul
{
	struct RegisteredCallback
	{
		Events eEventType; // The event this callback is registered for.
		CallbackId uiCallbackId; // The id of the associated callback.
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
		EventListener(EventListener&& oOtherListener);

		~EventListener();

		EventListener& operator=(EventListener&& oOtherListener);

		/*
		Adds a new event to the end of event queue to be dispatched.
		*/
		void QueueEvent(Events eEventType, void* pData);

		/*
		Adds a new callback to be called when the given event is triggered.

		@param eEventType - The event to register the callback for.

		@param fnCallback - The callback function to be registered.

		@return CallbackId containing the unique callback id.
		*/
		CallbackId RegisterCallback(Events eEventType, EventCallback fnCallback);

		/*
		Unregisters the callback with the given id from the given event.

		@param eEventType - The event to unregister the callback from.

		@param uiId - The id of the callback to unregister.
		*/
		void UnregisterCallback(Events eEventType);

		EventListener(const EventListener&) = delete;
		EventListener& operator=(const EventListener&) = delete;

	private:
		Vector<RegisteredCallback> _hRegisteredCallbacks; // The callback Id for each registeredevent.
	};

}