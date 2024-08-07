//
// Created by Luca Schmitt on 04.08.24.
//


#include "Screen.h"
#include <iostream>

Screen::Screen(std::string name, sf::RenderWindow &window)
: name_(std::move(name)), window_(window), rootContainer_("Root", sf::Vector2f(1, 1)) {
    std::cout << "Screen name: " << name_ << std::endl;
    // Initialize the root container
    rootContainer_.setPosition(sf::Vector2f(0, 0));
    rootContainer_.setParentSize(static_cast<sf::Vector2f> (window.getSize()));
}

// Implementation of move constructor
Screen::Screen(Screen&& other) noexcept
        : app_(std::move(other.app_)),
          name_(std::move(other.name_)),
          window_(other.window_),
          rootContainer_(std::move(other.rootContainer_)) {}

void Screen::draw(sf::RenderWindow &window) {
    rootContainer_.draw(window);
}

void Screen::handleEvent(sf::Event &event) {
    rootContainer_.handleEvent(event);
}

void Screen::setPaddingProportions(sf::Vector2f paddingProportions) {
    rootContainer_.setPaddingProportions(paddingProportions);
}

void Screen::setSpacingProportions(sf::Vector2f spacingProportions) {
    rootContainer_.setSpacingProportions(spacingProportions);
}

void Screen::setOutline(sf::Color color, float thickness) {
    rootContainer_.setOutline(color, thickness);
}

void Screen::setBackground(sf::Color color) {
    rootContainer_.setBackground(color);
}

void Screen::setAlignment(Alignment alignment) {
    rootContainer_.setAlignment(alignment);
}

void Screen::setLayoutOrientation(LayoutOrientation layoutOrientation) {
    rootContainer_.setLayoutOrientation(layoutOrientation);
}

void Screen::setApplication(std::shared_ptr<Application> app) {
    // Move the shared application pointer to the screen class
    app_ = std::move(app);
}

void Screen::addContainer(std::unique_ptr<Container> container) {
    // Add parent size information to the container
    container->setParentSize(static_cast<sf::Vector2f> (window_.getSize()));

    // Add the container
    rootContainer_.addContainer(std::move(container));
}

void Screen::addUIComponent(std::unique_ptr<UIComponent> component) {
    // Add parent size information to the UIComponent
    component->setParentSize(static_cast<sf::Vector2f> (window_.getSize()));

    // Add the UIComponent
    rootContainer_.addUIComponent(std::move(component));
}

void Screen::removeContainer(const std::string &containerName) {
    rootContainer_.removeContainer(containerName);
}

void Screen::removeUIComponent(const std::string &componentName) {
    rootContainer_.removeUIComponent(componentName);
}

const std::string &Screen::getName() {
    return name_;
}


