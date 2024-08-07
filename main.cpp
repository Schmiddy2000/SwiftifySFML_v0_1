#include <iostream>
#include <SFML/Graphics.hpp>
#include "Container.h"
#include "Screen.h"
#include "Application.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "UI Demo");
    // Set the frame rate limit to x FPS
    window.setFramerateLimit(10);

    // Ensure that the Application is created as a shared_ptr
    auto myApp = std::make_shared<Application>(window);

    // Create a screen and set a background color
    Screen myScreen("StartScreen", window);
    myScreen.setBackground(sf::Color::Blue);

    // Create and add a container
    Container myContainer("Container1", sf::Vector2f(0.5, 0.5));
    myContainer.setBackground(sf::Color::Magenta);
    myScreen.addContainer(std::make_unique<Container>(std::move(myContainer)));

    // Add the screen to the application
    myApp->addScreen(std::make_unique<Screen>(std::move(myScreen)));

    // Run the application
    myApp->run();

    return 0;
}
