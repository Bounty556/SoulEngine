/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 12/26/2020
*/

#include "EventBus.h"

namespace Soul
{
	UniqueHandle<Queue<Event>> EventBus::_shEventQueue;
	UniqueHandle<Vector<Callback>> EventBus::_shRegisteredCallbacks;
	CallbackId EventBus::_uiCallbackCount = 0;
	bool EventBus::_bIsSetup = false;

	void EventBus::StartUp(ArraySize uiEventCount)
	{
		Assert(!_bIsSetup);
		_shEventQueue = MemoryManager::Allocate<Queue<Event>>(uiEventCount);
		_shRegisteredCallbacks =
			MemoryManager::AllocateArray<Vector<Callback>>((ArraySize)Events::EventTotal);

		for (ArraySize i = 0; i < (ArraySize)Events::EventTotal; ++i)
		{
			Vector<Callback> oVec(4);
			_shRegisteredCallbacks[i] = std::move(oVec);
		}

		_bIsSetup = true;
	}

	void EventBus::Shutdown()
	{
		Assert(_bIsSetup);
		_shEventQueue.Deallocate();
		_shRegisteredCallbacks.Deallocate();
		_bIsSetup = false;
	}

	void EventBus::QueueEvent(Events eEventType, void* pData)
	{
		Event oEvent{ eEventType, pData };
		_shEventQueue->Push(oEvent);
	}

	void EventBus::DispatchEvents()
	{
		/*
		Dispatch up to the first "uiEventCount" events to all registered
		callbacks for that event.
		*/
		while (_shEventQueue->GetLength() > 0)
		{
			Event oEvent = _shEventQueue->Pop();
			Vector<Callback>& oEventCallbacks =
				_shRegisteredCallbacks[(ArraySize)oEvent.eEventType];
			for (ArraySize j = 0; j < oEventCallbacks.Length(); ++j)
			{
				oEventCallbacks[j].fnCallback(oEvent.pData);
			}
		}
	}

	CallbackId EventBus::RegisterCallback(Events eEventType, EventCallback fnCallback)
	{
		/*
		Add new callback to registered callbacks vector and return the
		CallbackId.
		*/
		Vector<Callback>& oEventCallbacks =
			_shRegisteredCallbacks[(ArraySize)eEventType];
		CallbackId uiCallbackId = _uiCallbackCount++;
		Callback oCallback{ fnCallback, uiCallbackId };
		oEventCallbacks.Push(oCallback);
		return uiCallbackId;
	}

	void EventBus::UnregisterCallback(Events eEventType, CallbackId uiId)
	{
		/*
		Find callback with given Id and remove from vector.
		*/
		Vector<Callback>& oEventCallbacks =
			_shRegisteredCallbacks[(ArraySize)eEventType];
		for (ArraySize i = 0; i < oEventCallbacks.Length(); ++i)
		{
			if (oEventCallbacks[i].uiCallbackId == uiId)
			{
				oEventCallbacks.Remove(i);
				break;
			}
		}
	}
}