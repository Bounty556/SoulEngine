/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 12/23/2020
*/

#pragma once

#include <Events/EventTypes.h>
#include <UtilsLib/CommonTypes.h>

namespace Soul
{
	class EventBus
	{
	public:

		/*
		Adds a new event to the end of event queue to be dispatched.
		*/
		void QueueEvent(Events eEventType, void* pData);

		/*
		Dispatches the first N number of events to all attached event listeners.
		
		@param uiEventCount - Number of events to dispatch.
		*/
		void DispatchEvents(UInt8 uiEventCount);

		EventBus() = delete;

	private:

	};
}