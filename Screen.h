//
// Created by Luca Schmitt on 03.08.24.
//

#ifndef SWIFTIFYSFML_V0_1_SCREEN_H
#define SWIFTIFYSFML_V0_1_SCREEN_H

#include <SFML/Graphics.hpp>
#include "Enumerations.h"
#include "Container.h"
// #include "Application.h"

// Forward declaration of Application
class Application;

/*
 * Abstract Screen class. Has essential virtual function definitions and can be extended to fit the specific needs
 * of a certain application screen. It should also have access to the main application via a shared pointer.
 */
class Screen {
public:
    // Constructor and destructor
    Screen(std::string name, sf::RenderWindow& window);
    virtual ~Screen() = default;

    // Move constructor
    Screen(Screen&& other) noexcept;

    // Draw the content of all child views to the screen by calling their draw functions
    virtual void draw(sf::RenderWindow& window);

    virtual void handleEvent(sf::Event& event);

    // --- Setter functions ---

    // Set the padding proportions
    void setPaddingProportions(sf::Vector2f paddingProportions);

    // Set the spacing proportions
    void setSpacingProportions(sf::Vector2f spacingProportions);

    // Set the background information
    void setOutline(sf::Color color, float thickness);
    void setBackground(sf::Color color);

    // Set the alignment
    void setAlignment(Alignment alignment);

    // Set the layout
    void setLayoutOrientation(LayoutOrientation layoutOrientation);

    // Set a shared pointer to the main application instance
    void setApplication(std::shared_ptr<Application> app);

    // Add a container or UI component to the list of nested containers
    void addContainer(std::unique_ptr<Container> container);
    void addUIComponent(std::unique_ptr<UIComponent> component);

    // Remove a container or UIComponent by name
    void removeContainer(const std::string& containerName);
    void removeUIComponent(const std::string& componentName);

    // --- Getter functions ---

    const std::string& getName();

protected:
    // Pointer to the main application handling the application interactions on the highest level
    std::shared_ptr<Application> app_;

    // Name of the container for access and error messages
    std::string name_;

    // Window to draw the screen content to
    sf::RenderWindow& window_;

    // Root container
    Container rootContainer_;
};

#endif //SWIFTIFYSFML_V0_1_SCREEN_H
