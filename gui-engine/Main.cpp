#include <iostream>
#include "GUICore.h"

class Test : public gui::IEventListener
{
public:

    void handleGUIEvent(gui::EventType type, const gui::Component* component) override
    {
        switch (type)
        {
        case gui::EventType::Click:
            std::cout << "click!\n";
            break;
        case gui::EventType::MouseEnter:
            std::cout << "enter!\n";
            break;
        case gui::EventType::MouseLeave:
            std::cout << "leave!\n";
            break;
        }
    }
};

class Test2 : public gui::IEventListener
{
public: 

    void handleGUIEvent(gui::EventType type, const gui::Component* component) override
    {
        switch (type)
        {
        case gui::EventType::Click:
            std::cout << "text click!\n";
            break;
        case gui::EventType::MouseEnter:
            std::cout << "text enter!\n";
            break;
        case gui::EventType::MouseLeave:
            std::cout << "text leave!\n";
            break;
        }
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    Test test;
    Test2 test2;

    gui::Button btn(sf::Vector2f (100, 200 ), sf::Vector2f( 500, 350 ), &window);
    btn.addListener(&test);

    btn.setText("Hello!");
    btn.setFontSize(50);

    gui::TextBlock txt(sf::Vector2f(0, 0), "this is text!", &window);
    txt.addListener(&test2);
    txt.interactivity = true;

    txt.setColor(sf::Color::Green, sf::Color::Red);

    gui::ProgressBar bar(sf::Vector2f(400, 100), sf::Vector2f(200, 50), &window);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            bar.setProgress(bar.getProgress() + 1);
            std::cout << "Y\n";
        }

        window.clear();
        window.draw(btn);
        window.draw(txt);
        window.draw(bar);
        window.display();
    }

    return 0;
}
