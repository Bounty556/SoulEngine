/*
Transfers events to all registered event listeners in queue order.
@file EventBus.h
@author Jacob Peterson
@edited 4/18/21
*/

#include "EventBus.h"

#include <UtilsLib/Logger.h>

namespace Soul
{
	UniqueHandle<Queue<Event>> EventBus::m_EventQueue;
	UniqueHandle<Vector<Callback>> EventBus::m_RegisteredCallbacks;
	CallbackId EventBus::m_CallbackCount = 0;
	bool EventBus::m_IsSetup = false;

	void EventBus::StartUp(ArraySize eventCount)
	{
		Assert(!m_IsSetup);
		m_EventQueue = MemoryManager::Allocate<Queue<Event>>(eventCount);
		m_RegisteredCallbacks =
			MemoryManager::AllocateArray<Vector<Callback>>((ArraySize)Events::EventTotal);
		m_RegisteredCallbacks.SetImmovable(true);

		for (ArraySize i = 0; i < (ArraySize)Events::EventTotal; ++i)
		{
			Vector<Callback> vec(4);
			m_RegisteredCallbacks[i] = std::move(vec);
		}

		m_IsSetup = true;
	}

	void EventBus::Shutdown()
	{
		Assert(m_IsSetup);
		m_EventQueue.Deallocate();
		m_RegisteredCallbacks.Deallocate();
		m_IsSetup = false;
	}

	void EventBus::QueueEvent(Events eventType, void* data)
	{
		Event oEvent{ eventType, data };
		m_EventQueue->Push(oEvent);
	}

	void EventBus::DispatchEvents()
	{
		/*
		Dispatch up to the first "eventCount" events to all registered
		callbacks for that event.
		*/
		while (m_EventQueue->GetLength() > 0)
		{
			Event currentEvent = m_EventQueue->Pop();
			Vector<Callback>& eventCallbacks =
				m_RegisteredCallbacks[(ArraySize)currentEvent.eventType];
			for (ArraySize j = 0; j < eventCallbacks.Length(); ++j)
			{
				eventCallbacks[j].callbackFunction(currentEvent.pointerToData);
			}
		}
	}

	CallbackId EventBus::RegisterCallback(Events eventType, EventCallback callback)
	{
		/*
		Add new callback to registered callbacks vector and return the
		CallbackId.
		*/
		Vector<Callback>& eventCallbacks =
			m_RegisteredCallbacks[(ArraySize)eventType];
		CallbackId callbackId = m_CallbackCount++;
		Callback tempCallback{ callback, callbackId };
		eventCallbacks.Push(tempCallback);
		return callbackId;
	}

	void EventBus::UnregisterCallback(Events eventType, CallbackId Id)
	{
		/*
		Find callback with given Id and remove from vector.
		*/
		Vector<Callback>& eventCallbacks =
			m_RegisteredCallbacks[(ArraySize)eventType];
		for (ArraySize i = 0; i < eventCallbacks.Length(); ++i)
		{
			if (eventCallbacks[i].callbackId == Id)
			{
				eventCallbacks.Remove(i);
				break;
			}
		}
	}
}