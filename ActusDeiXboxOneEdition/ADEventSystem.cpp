#include "pch.h"
#include "ADEventSystem.h"

bool ADEvents::ADEventSystem::AlreadyRegistered(std::string eventId, Listener * _client)
{
	bool alreadyRegistered = false;

	std::pair<std::multimap<std::string, Listener*>::iterator,
		std::multimap<std::string, Listener*>::iterator> range;

	range = listeners.equal_range(eventId);

	for (std::multimap<std::string, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		if ((*iter).second == _client)
		{
			alreadyRegistered = true;
			break;
		}
	}
	return alreadyRegistered;
}

void ADEvents::ADEventSystem::DispatchEvent(ADEvent* _event)
{
	std::pair<std::multimap<std::string, Listener*>::iterator,
		std::multimap<std::string, Listener*>::iterator> range;

	range = listeners.equal_range(_event->EventId());

	for (std::multimap<std::string, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		(*iter).second->HandleEvent(_event);
	}
}

ADEvents::ADEventSystem* ADEvents::ADEventSystem::Instance()
{
	static ADEventSystem instance;
	return &instance;
}

void ADEvents::ADEventSystem::PrintData()
{
	for (std::multimap<std::string, Listener*>::iterator iter = listeners.begin(); iter != listeners.end(); iter++)
	{
		//Logger::log(iter->first);
		//Logger::log(iter->second->ToString());
	}
}

void ADEvents::ADEventSystem::RegisterClient(std::string _eventId, Listener* _client)
{
	if (!_client || AlreadyRegistered(_eventId, _client))
	{
		return;
	}

	listeners.insert(make_pair(_eventId, _client));
}

void ADEvents::ADEventSystem::UnregisterClient(std::string _eventId, Listener* _client)
{
	std::pair < std::multimap<std::string, Listener*>::iterator, std::multimap<std::string, Listener*>::iterator> range;
	range = listeners.equal_range(_eventId);

	for (std::multimap<std::string, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		if ((*iter).second == _client)
		{
			iter = listeners.erase(iter);
			break;
		}
	}
}

void ADEvents::ADEventSystem::UnregisterAll(Listener* _client)
{
	std::multimap<std::string, Listener*>::iterator iter = listeners.begin();

	while (iter != listeners.end())
	{
		if ((*iter).second == _client)
		{
			iter = listeners.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void ADEvents::ADEventSystem::SendEvent(std::string _eventId, void* _data)
{
	ADEvent newEvent(_eventId, _data);
	currentEvents.push_back(newEvent);
}

void ADEvents::ADEventSystem::ProcessEvents()
{
	while (currentEvents.size())
	{
		DispatchEvent(&currentEvents.front());
		currentEvents.pop_front();
	}
}

void ADEvents::ADEventSystem::ClearEvents()
{
	currentEvents.clear();
}

void ADEvents::ADEventSystem::Shutdown()
{
	listeners.clear();
	currentEvents.clear();
}
