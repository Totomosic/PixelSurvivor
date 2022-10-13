#pragma once
#include <vector>
#include <functional>

namespace Pixel
{

	template<typename T>
	struct Event
	{
	public:
		T Data;
		bool Handled = false;

	public:
		inline void StopPropagation() { Handled = true; }
	};

	template<typename T>
	class EventEmitter
	{
	public:
		using ListenerFunction = std::function<void(Event<T>&)>;

	private:
		struct EventListener
		{
		public:
			int Id;
			ListenerFunction Listener;
		};

	private:
		mutable int m_NextListenerId = 0;
		mutable std::vector<EventListener> m_Listeners;

	public:
		EventEmitter() = default;

		inline int AddEventListener(const ListenerFunction& listener) const
		{
			int id = m_NextListenerId++;
			m_Listeners.push_back({ id, listener });
			return id;
		}

		inline void RemoveEventListener(int listenerId) const
		{
			auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [listenerId](const EventListener& l) { return l.Id == listenerId });
			if (it != m_Listeners.end())
				m_Listeners.erase(it);
		}

		inline Event<T> Trigger(T&& data) const
		{
			Event<T> evt = { std::move(data) };
			for (const EventListener& listener : m_Listeners)
			{
				listener.Listener(evt);
				if (evt.Handled)
					break;
			}
			return evt;
		}
	};

}
