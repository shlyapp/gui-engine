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

	enum class HorizontalAligment
	{
		Center,
		Left,
		Right
	};

	enum class VerticalAligment
	{
		Bottom,
		Center,
		Left,
		Right
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
	public:

		bool visibility;
		bool activity;

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
			visibility(true),
			activity(true),
			event_(EventType::MouseLeave)
		{

		}

		Component(sf::RenderWindow* window) :
			position_({ 0, 0 }),
			size_({ 0, 0 }),
			window_(window),
			visibility(true),
			activity(true),
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
				std::cout << "click!\n";
			}
		}

		virtual void enter() const
		{
			if (event_ != EventType::MouseEnter)
			{
				event_ = EventType::MouseEnter;
				notifyListeners();
				std::cout << "enter!\n";
			}
		}

		virtual void leave() const
		{
			if (event_ != EventType::MouseLeave)
			{
				event_ = EventType::MouseLeave;
				notifyListeners();
				std::cout << "leave!\n";
			}
		}

		virtual void update() const
		{
			if (activity)
			{
				sf::Vector2f mouse_pos = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

				//std::cout << size_.x << "\t" << size_.y << "\t" << mouse_pos.x << "\t" << mouse_pos.y << std::endl;

				if (sf::IntRect(position_.x, position_.y, size_.x, size_.y).contains(mouse_pos.x, mouse_pos.y))
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						if (event_ != EventType::Click)
						{
							click();
						}
						
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
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override = 0;

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

		virtual void setPosition(VerticalAligment aligment)
		{
			sf::Vector2f position;
			switch (aligment)
			{
			case gui::VerticalAligment::Bottom:
				break;
			case gui::VerticalAligment::Center:
				position.x = position_.x;
				position.y = window_->getSize().y / 2 - size_.y / 2;
				setPosition(position);
				break;
			case gui::VerticalAligment::Left:
				break;
			case gui::VerticalAligment::Right:
				break;
			default:
				break;
			}
		}

		void setPosition(HorizontalAligment aligment)
		{

		}

	};

	class Button : public Component
	{
	protected:

		Button(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) :
			Component(position, size, window)
		{
			
		}

	public:

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override = 0;

	};

	class ColorButton : public Button
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
			text_.setCharacterSize(20.0f);
			text_.setOrigin(text_.getLocalBounds().width / 2, text_.getLocalBounds().height / 2);

			text_.setPosition(position_.x + size_.x / 2, position_.y + size_.y / 2);
			text_.setFillColor(sf::Color::White);
		}

		void updatePosition()
		{
			text_.setOrigin(text_.getLocalBounds().width / 2, text_.getLocalBounds().height / 2);
			text_.setPosition(position_.x + size_.x / 2, position_.y + size_.y / 2);
		}

	public:

		ColorButton(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window):
			Button(position, size, window),
			colors_({sf::Color::Green, sf::Color::Red})
		{
			InitRect();
			InitText();
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::update();
			if (visibility)
			{
				target.draw(rect_);
				target.draw(text_);
			}
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

		void setColor(sf::Color disactive, sf::Color active)
		{
			colors_ = { disactive, active };
		}

	};

	class TextureButton : public Button
	{
	private:

		sf::Vector2<sf::Texture*> textures_;
		mutable sf::Sprite btn_sprite_;
		
		void enter() const override
		{
			Component::enter();
			btn_sprite_.setTexture(*textures_.y);
		}

		void leave() const override
		{
			Component::leave();
			btn_sprite_.setTexture(*textures_.x);
		}

		void InitTextures()
		{
			textures_.x->loadFromFile("res/btn_1.png");
			textures_.y->loadFromFile("res/btn_2.png");
			btn_sprite_.setTexture(*textures_.x);
		}

	public: 

		TextureButton(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) :
			Button(position, size, window),
			textures_({ new sf::Texture(), new sf::Texture() })
		{
			InitTextures();
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::update();
			if (visibility)
			{
				target.draw(btn_sprite_);
			}
		}

		void setPosition(sf::Vector2f position) override
		{
			Component::setPosition(position);
			btn_sprite_.setPosition(position);
		}

	};

	class StatusButton : virtual public Button
	{
	private:

		std::vector<sf::Texture*> textures_;
		mutable sf::Sprite btn_sprite_;

		mutable int iter_num_;

		void click() const override
		{
			Component::click();

			iter_num_++;
			if (iter_num_ == textures_.size())
			{
				iter_num_ = 0;
			}

			btn_sprite_.setTexture(*textures_[iter_num_]);
		}

	public:

		StatusButton(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) :
			Button(position, size, window),
			iter_num_(0)
		{

		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::update();
			if (visibility)
			{
				target.draw(btn_sprite_);
				//std::cout << iter_num_ << std::endl;
				//std::cout << btn_sprite_.getTexture() << std::endl;
			}
		}

		void setPosition(sf::Vector2f position) override
		{
			Component::setPosition(position);
			btn_sprite_.setPosition(position);
		}

		void addTexture(sf::Texture* texture)
		{
			textures_.push_back(texture);
			if (btn_sprite_.getTexture() == nullptr)
			{
				std::cout << "YES\n";
				btn_sprite_.setTexture(*textures_[iter_num_]);
			}
		}

		void setPosition(VerticalAligment aligment)
		{
			Component::setPosition(aligment);
		}

	};

	class TextBlock : public Component
	{
	public:

		bool interactivity;

	private:

		mutable sf::Text text_;
		sf::Font font_;

		sf::Vector2 <sf::Color> colors_;
		
		sf::Vector2f m_sfml_bounds = { 2, 12 };

		void InitText(const std::string text)
		{
			font_.loadFromFile("res/font.ttf");

			text_.setFont(font_);
			text_.setString(text);
			text_.setCharacterSize(50.0f);

			text_.setPosition(position_.x, position_.y);
			size_ = { sf::Vector2f{text_.getLocalBounds().width, text_.getLocalBounds().height } + m_sfml_bounds };
			text_.setFillColor(colors_.x);
		}

		void enter() const override
		{
			Component::enter();

			if (interactivity)
			{
				text_.setFillColor(colors_.y);
			}		
		}

		void leave() const override
		{
			Component::leave();

			if (interactivity)
			{
				text_.setFillColor(colors_.x);
			}		
		}

	public:

		TextBlock(sf::Vector2f position, std::string text, sf::RenderWindow* window) :
			Component(window),
			colors_({ sf::Color::White, sf::Color::Black }),
			interactivity(false)
		{
			Component::setPosition(position);
			InitText(text);
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::update();
			if (visibility)
			{
				target.draw(text_);
			}	
		}

		void setText(const std::string text)
		{
			text_.setString(text);
			size_ = { sf::Vector2f{text_.getLocalBounds().width, text_.getLocalBounds().height } + m_sfml_bounds };
		}

		void setColor(sf::Color disactive, sf::Color active)
		{
			colors_ = { disactive, active };
			text_.setFillColor(colors_.x);
		}

		void setColor(sf::Color color)
		{
			colors_.x = color;
			text_.setFillColor(colors_.x);
		}

	};

	class ProgressBar : public Component
	{
	private:

		sf::RectangleShape border_;
		sf::RectangleShape progress_bar_;

		sf::Vector2<int> range_;
		int progress_;
		float step_;

		void initRects()
		{
			border_.setPosition(position_);
			border_.setSize(size_);
			border_.setFillColor(sf::Color::Transparent);
			border_.setOutlineThickness(-2);
			
			progress_bar_.setPosition(position_ + sf::Vector2f(2, 2));
			progress_bar_.setSize(sf::Vector2f(0, size_.y - 4));
			progress_bar_.setFillColor(sf::Color::Green);
		}

	public:

		ProgressBar(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) :
			Component(position, size, window),
			range_({ 0, 100 }),
			progress_(0),
			step_((size.x - 4) / range_.y)
		{
			initRects();
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			Component::update();
			target.draw(border_);
			target.draw(progress_bar_);
		}

		void setPosition(const sf::Vector2f position) override
		{
			Component::setPosition(position);

			border_.setPosition(position_);
			progress_bar_.setPosition(position_ + sf::Vector2f(2, 2));
		}

		void setColor(sf::Color color)
		{
			progress_bar_.setFillColor(color);
		}

		void setOutlineColor(sf::Color color)
		{
			border_.setFillColor(color);
		}

		void setProgress(const int value)
		{
			if ((value > 100) or ((progress_ + value) < 0))
			{
				return;
			}

			progress_ = value;
			progress_bar_.setSize(sf::Vector2f(step_*progress_, size_.y - 4));
		}

		int getProgress() const
		{
			return progress_;
		}

	};
}