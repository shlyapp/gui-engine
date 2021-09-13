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
	protected:

		IEventListener()
		{

		}

	public:

		virtual void handleGUIEvent(EventType type, const Component* component) = 0;

	};

	class Component : public sf::Drawable
	{
	protected:

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

		Component(sf::RenderWindow* window) :
			position_({ 0, 0 }),
			size_({ 0, 0 }),
			window_(window),
			event_(EventType::MouseLeave)
		{

		}

		void notifyListeners() const
		{
			for (auto listener : listeners_)
			{
				listener->handleGUIEvent(event_, this);
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

	public:

		void addListener(IEventListener* listener)
		{
			listeners_.push_back(listener);
		}

		void remoteListener(IEventListener* listener)
		{
			listeners_.remove(listener);
		}

		sf::Vector2f getPosition() const
		{
			return position_;
		}

		sf::Vector2f getSize() const
		{
			return size_;
		}

		virtual void setPosition(const sf::Vector2f position)
		{
			position_ = position;
		}

	};

	class Button : public Component
	{
	private:

		sf::Vector2<sf::Color> colors_;
		mutable sf::RectangleShape rect_;

		sf::Text text_;
		sf::Font font_;

		void enter() const override
		{
			Component::enter();
			rect_.setFillColor(colors_.y);
		}

		void leave() const override
		{
			Component::leave();
			rect_.setFillColor(colors_.x);
		}

		void InitRect()
		{
			rect_.setPosition(position_);
			rect_.setSize(size_);
			rect_.setFillColor(colors_.x);
		}

		void InitText()
		{
			font_.loadFromFile("res/font.ttf");

			text_.setFont(font_);
			text_.setString("button");
			text_.setCharacterSize(50.0f);
			text_.setOrigin(text_.getLocalBounds().width / 2, text_.getLocalBounds().height / 2);

			text_.setPosition(position_.x + size_.x/2, position_.y + size_.y/2);
			text_.setFillColor(sf::Color::White);
		}

		void updatePosition()
		{
			text_.setOrigin(text_.getLocalBounds().width / 2, text_.getLocalBounds().height / 2);
			text_.setPosition(position_.x + size_.x / 2, position_.y + size_.y / 2);
		}

	public:

		Button(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) :
			Component(position, size, window),
			colors_({ sf::Color::Green, sf::Color::Red })
		{
			InitRect();
			InitText();
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::draw(target, animation_state);
			target.draw(rect_);
			target.draw(text_);
		}

		void setPosition(sf::Vector2f position) override
		{
			Component::setPosition(position);
			text_.setPosition(position_.x + size_.x / 2, position_.y + size_.y / 2);
		}

		void setText(const std::string text)
		{
			text_.setString(text);
			updatePosition();
		}

		void setFontSize(const int size)
		{
			text_.setCharacterSize(size);
			updatePosition();
		}

		void setColors(sf::Color disactive, sf::Color active)
		{
			colors_ = { disactive, active };
		}

	};

	class TextBlock : public Component
	{
	private:

		sf::Text text_;
		sf::Font font_;

		void InitText(const std::string text)
		{
			font_.loadFromFile("res/font.ttf");

			text_.setFont(font_);
			text_.setString(text);
			text_.setCharacterSize(50.0f);

			text_.setPosition(position_.x, position_.y);
			size_ = { text_.getLocalBounds().width, text_.getLocalBounds().height };
			text_.setFillColor(sf::Color::White);
		}

	public:

		TextBlock(sf::Vector2f position, std::string text, sf::RenderWindow* window) :
			Component(window)
		{
			Component::setPosition(position);
			InitText(text);
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::draw(target, animation_state);
			target.draw(text_);
		}

		void setText(const std::string text)
		{
			text_.setString(text);
			size_ = { text_.getLocalBounds().width, text_.getLocalBounds().height };
		}

	};

}