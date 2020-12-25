/*
Manages adding and removing callbacks with the EventBus to provide a thin
layer of abstraction and automation.
@file EventListener.h
@author Jacob Peterson
@edited 12/24/2020
*/

#include "EventListener.h"

namespace Soul
{
	EventListener::EventListener() :
		_hRegisteredCallbacks(4)
	{
	
	}

	EventListener::EventListener(EventListener&& oOtherListener) :
		_hRegisteredCallbacks(std::move(oOtherListener._hRegisteredCallbacks))
	{

	}

	EventListener::~EventListener()
	{
		for (ArraySize i = 0; i < _hRegisteredCallbacks.Length(); ++i)
		{
			RegisteredCallback& oCallback = _hRegisteredCallbacks[i];
			EventBus::UnregisterCallback(oCallback.eEventType, oCallback.uiCallbackId);
		}
	}

	EventListener& EventListener::operator=(EventListener&& oOtherListener)
	{
		_hRegisteredCallbacks = std::move(oOtherListener._hRegisteredCallbacks);
		return *this;
	}

	CallbackId EventListener::RegisterCallback(Events eEventType, EventCallback fnCallback)
	{
		RegisteredCallback oCallback =
		{ eEventType, EventBus::RegisterCallback(eEventType, fnCallback) };
		_hRegisteredCallbacks.Push(oCallback);
		return oCallback.uiCallbackId;
	}

	void EventListener::UnregisterCallback(Events eEventType)
	{
		for (ArraySize i = 0; i < _hRegisteredCallbacks.Length(); ++i)
		{
			RegisteredCallback& oCallback = _hRegisteredCallbacks[i];
			if (oCallback.eEventType == eEventType)
			{
				EventBus::UnregisterCallback(eEventType, oCallback.uiCallbackId);
				break;
			}
		}
	}
}