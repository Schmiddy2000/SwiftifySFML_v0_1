//
// Created by Anna Knecht on 03.08.24.
//

#ifndef SWIFTIFYSFML_V0_1_APPLICATION_H
#define SWIFTIFYSFML_V0_1_APPLICATION_H

#include <memory>
#include <vector>
#include "Screen.h"

// Forward declaration of the screen class
// class Screen;

/*
 * Abstract Application class. Handles high-level functionalities and interactions such as screen transitions and
 * running the main loop. It can be subclassed and extended with additional functionalities.
 */
class Application : public std::enable_shared_from_this<Application> {
public:
    // Constructor and destructor
    explicit Application(sf::RenderWindow& window);
    virtual ~Application() = default;

    std::shared_ptr<Application> getPtr();

    // Run the main loop of the application
    virtual void run();

    // Transition to a new screen via the screen's name by updating the current screen
    virtual void transitionToScreen(const std::string& name);

    // Add a new screen
    virtual void addScreen(std::unique_ptr<Screen> screen);

    // Remove a screen (is this necessary?)
    virtual void removeScreen(const std::string& name);

    // Get a reference to a screen by name
    Screen& getScreen(const std::string& name);

    // Set the current screen to the specified startScreen
    void setStartScreen(const std::string& name);

protected:
    // Check if a screen with a given name exists. Returns the index if it does. Else returns -1.
    int checkIfNameExists(const std::string& name);

    // Window for drawing graphics
    sf::RenderWindow& window_;

    // Store the screens in the application
    std::vector<std::unique_ptr<Screen>> screens_;

    // Pointer to the current screen pointer
    std::unique_ptr<Screen>* currentScreen_;
};

#endif //SWIFTIFYSFML_V0_1_APPLICATION_H
