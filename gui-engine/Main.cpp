#include <iostream>
#include "GUICore.h"

class Test : public gui::IEventListener, public sf::Drawable
{
private:

    gui::ColorButton* btn_up_;
    gui::ColorButton* btn_down_;
    gui::TextureButton* btn_;
    gui::ProgressBar* bar_;

public:

    Test(sf::RenderWindow* window) :
        btn_up_(new gui::ColorButton(sf::Vector2f(400, 100), sf::Vector2f(300, 150), window)),
        btn_down_(new gui::ColorButton(sf::Vector2f(700, 100), sf::Vector2f(300, 150), window)),
        bar_(new gui::ProgressBar(sf::Vector2f(100, 100), sf::Vector2f(300, 150), window)),
        btn_(new gui::TextureButton(sf::Vector2f(0, 0), sf::Vector2f(249, 62), window))
    {
        btn_up_->setText("Up bar");
        btn_up_->setFontSize(50);
        btn_up_->addListener(this);

        btn_down_->setText("Down bar");
        btn_down_->setFontSize(50);
        btn_down_->addListener(this);

        btn_->setPosition({ 500, 500 });
    }

    void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
    {
        target.draw(*btn_up_);
        target.draw(*btn_down_);
        target.draw(*bar_);
        target.draw(*btn_);
    }

    void handleGUIEvent(gui::EventType type, const gui::Component* component) override
    {
        if (type == gui::EventType::Click)
        {
            if (component == btn_up_)
            {
                bar_->setProgress(bar_->getProgress() + 10);
            }
            else
            {
                bar_->setProgress(bar_->getProgress() - 10);
            }
        }
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    Test test(&window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        window.draw(test);
        window.display();
    }

    return 0;
}
