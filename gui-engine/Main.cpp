#include <iostream>
#include "GUICore.h"

class Test : public gui::IEventListener
{
public:

    void updateByGUIEvent(gui::EventType type, const gui::Component* component) override
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
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    Test test;

    gui::Component component(sf::Vector2f(0, 0), sf::Vector2f(100, 100), &window);
    component.addListener(&test);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(component);
        window.display();
    }

    return 0;
}
