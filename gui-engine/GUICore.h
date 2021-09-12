#pragma once

#include <SFML/Graphics.hpp>
#include <list>

namespace gui
{
	class Component;

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

	class Component : public sf::Drawable
	{
	public:

		std::list<IEventListener*> listeners_;

		mutable EventType event_;

		sf::RenderWindow* window_;

		sf::Vector2f position_;
		sf::Vector2f size_;

		Component(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) :
			position_(position),
			size_(size),
			window_(window),
			event_(EventType::MouseLeave)
		{

		}

		void notifyListeners() const
		{
			for (auto listener : listeners_)
			{
				listener->updateByGUIEvent(event_, this);
			}
		}

		virtual void click() const
		{
			if (event_ != EventType::Click)
			{
				event_ = EventType::Click;
				notifyListeners();
			}
		}

		virtual void enter() const
		{
			if (event_ != EventType::MouseEnter)
			{
				event_ = EventType::MouseEnter;
				notifyListeners();
			}
		}

		virtual void leave() const
		{
			if (event_ != EventType::MouseLeave)
			{
				event_ = EventType::MouseLeave;
				notifyListeners();
			}
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			sf::Vector2f mouse_pos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

			if (sf::IntRect(position_.x, position_.y, size_.x, size_.y).contains(mouse_pos.x, mouse_pos.y))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					click();
				}
				else
				{
					enter();
				}
			}
			else
			{
				leave();
			}
		}

		void addListener(IEventListener* listener)
		{
			listeners_.push_back(listener);
		}

		void remoteListener(IEventListener* listener)
		{
			listeners_.remove(listener);
		}
	};

}

