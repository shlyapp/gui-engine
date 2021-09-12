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

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    Test test;

    gui::Button btn(sf::Vector2f (100, 200 ), sf::Vector2f( 500, 350 ), &window);
    btn.addListener(&test);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(btn);
        window.display();
    }

    return 0;
}
