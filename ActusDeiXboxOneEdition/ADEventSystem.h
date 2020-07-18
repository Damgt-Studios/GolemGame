#pragma once
#include <string>
#include <list>
#include <map>
//#include "Logger.h"

namespace ADEvents
{
	class ADEvent
	{
		std::string eventId;
		void* parameter;
	public:
		ADEvent(std::string _eventId, void* _parameter = 0)
		{
			eventId = _eventId;
			parameter = _parameter;
		}

		~ADEvent() {}

		inline std::string EventId() const { return eventId; }
		inline void* Parameter() { return parameter; }
	};

	class Listener
	{
	public:
		Listener() {}
		virtual ~Listener() {}
		virtual void HandleEvent(ADEvent* _event) = 0;
		virtual std::string ToString() { return ""; };
	};

	class ADEventSystem {
	private:

		std::multimap<std::string, Listener*> listeners;
		std::list<ADEvent> currentEvents;

		ADEventSystem() {};
		~ADEventSystem() { Shutdown(); };
		ADEventSystem(const ADEventSystem& _rhs) {};
		ADEventSystem& operator =(const ADEventSystem& _rhs) {};
		bool AlreadyRegistered(std::string eventId, Listener* _client);
		void DispatchEvent(ADEvent* _event);
	public:
		static ADEventSystem* Instance();

		void PrintData();
		void RegisterClient(std::string _eventId, Listener* _client);
		void UnregisterClient(std::string _eventId, Listener* _client);
		void UnregisterAll(Listener* _client);
		void SendEvent(std::string _eventId, void* _data = 0);
		void ProcessEvents();
		void ClearEvents();
		void Shutdown();
	};
}