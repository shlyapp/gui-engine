#pragma once

#include <SFML/Graphics.hpp>

namespace gui
{
	class Compoment;

	enum class EventType
	{
		Click,
		MouseEnter,
		MouseLeave
	};

	class IEventListener
	{
	public:

		virtual void updateByGUIEvent(EventType type, const Component* component) = 0;

	};

	class Component
	{
	private:

		sf::Vector2f position_;
		sf::Vector2f size_;

	};
}

