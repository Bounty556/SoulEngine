/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 12/23/2020
*/

#include "EventBus.h"

namespace Soul
{
	UniqueHandle<Queue<Event>> EventBus::_shEventQueue;
	bool EventBus::_bIsSetup = false;

	void EventBus::StartUp(ArraySize uiEventCount)
	{
		Assert(!_bIsSetup);
		_shEventQueue = MemoryManager::Allocate<Queue<Event>>(uiEventCount);
		_bIsSetup = true;
	}

	void EventBus::Shutdown()
	{
		Assert(_bIsSetup);
		_shEventQueue.Deallocate();
		_bIsSetup = false;
	}

	void EventBus::DispatchEvents(UInt8 uiEventCount)
	{
		/*
		TODO: Implement
		*/
	}
}