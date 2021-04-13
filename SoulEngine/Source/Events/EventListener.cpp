/*
Manages adding and removing callbacks with the EventBus to provide a thin
layer of abstraction and automation.
@file EventListener.h
@author Jacob Peterson
@edited 4/13/21
*/

#include "EventListener.h"

namespace Soul
{
	EventListener::EventListener() :
		m_RegisteredCallbacks(4)
	{
	
	}

	EventListener::EventListener(EventListener&& otherListener) :
		m_RegisteredCallbacks(std::move(otherListener.m_RegisteredCallbacks))
	{

	}

	EventListener::~EventListener()
	{
		for (ArraySize i = 0; i < m_RegisteredCallbacks.Length(); ++i)
		{
			RegisteredCallback& callback = m_RegisteredCallbacks[i];
			EventBus::UnregisterCallback(callback.eventType, callback.callbackId);
		}
	}

	EventListener& EventListener::operator=(EventListener&& otherListener)
	{
		m_RegisteredCallbacks = std::move(otherListener.m_RegisteredCallbacks);
		return *this;
	}

	void EventListener::QueueEvent(Events eventType, void* data)
	{
		EventBus::QueueEvent(eventType, data);
	}

	CallbackId EventListener::RegisterCallback(Events eventType, EventCallback callbackFunction)
	{
		RegisteredCallback callback =
		{ eventType, EventBus::RegisterCallback(eventType, callbackFunction) };
		m_RegisteredCallbacks.Push(callback);
		return callback.callbackId;
	}

	void EventListener::UnregisterCallback(Events eventType)
	{
		for (ArraySize i = 0; i < m_RegisteredCallbacks.Length(); ++i)
		{
			RegisteredCallback& callback = m_RegisteredCallbacks[i];
			if (callback.eventType == eventType)
			{
				EventBus::UnregisterCallback(eventType, callback.callbackId);
				break;
			}
		}
	}
}