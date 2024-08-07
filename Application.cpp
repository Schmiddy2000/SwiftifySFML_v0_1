//
// Created by Luca Schmitt on 06.08.24.
//

#include "Application.h"
#include <iostream>

Application::Application(sf::RenderWindow& window) : window_(window) {
    currentScreen_ = nullptr;
}

std::shared_ptr<Application> Application::getPtr() {
    return shared_from_this();
}

void Application::run() {
    // Raise a warning if no screen has been set as the start screen.
    if (!currentScreen_ or !*currentScreen_) {
        std::cerr << "No screen is set as the start screen" << std::endl;
        exit(1);
    }

    // Implement a basic main loop.
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
            (*currentScreen_)->handleEvent(event);

        }

        window_.clear();
        (*currentScreen_)->draw(window_);
        window_.display();
    }

}

void Application::transitionToScreen(const std::string &name) {
    // Check the screens for a screen matching the given name. When found, set it. Else, raise an error
    int checkStatus = checkIfNameExists(name);

    if (checkStatus > 0) {
        std::cerr << "Screen '" << name << "' was not found. Couldn't transition." << std::endl;
    } else {
        currentScreen_ = &(screens_[checkStatus]);
    }
}

void Application::addScreen(std::unique_ptr<Screen> screen) {
    // Add the screen to screens. Raise an error if a screen with that name already exists
    std::string name = screen->getName();
    int checkStatus = checkIfNameExists(name);

    if (checkStatus > 0) {
        std::cerr << "Screen '" << name << "' already exist. Please use a different name." << std::endl;
    } else {
        // Set the shared application pointer and add the screen to the list of screens
        screen->setApplication(getPtr());
        screens_.emplace_back(std::move(screen));

        // Set the first added screen as the start screen as a default
        if (screens_.size() == 1) {
            currentScreen_ = &screens_.back();
        }
    }


}

void Application::removeScreen(const std::string &name) {
    // Check if a screen with that name exists. If it does, remove it. Else raise an error
}

Screen& Application::getScreen(const std::string &name) {
    int checkStatus = checkIfNameExists(name);

    if (checkStatus > 0) {
        std::cerr << "Screen '" << name << "' was not found. Couldn't return it." << std::endl;
    } else {
        return *screens_[checkStatus];
    }

    // Should probably throw an error here
}

void Application::setStartScreen(const std::string &name) {
    int checkStatus = checkIfNameExists(name);

    if (checkStatus > 0) {
        std::cerr << "Screen '" << name << "' was not found. Couldn't set it as a start screen." << std::endl;
    } else {
        currentScreen_ = &(screens_[checkStatus]);
    }
}

int Application::checkIfNameExists(const std::string& name) {
    int index = 0;

    // Do the check and increment the index
    for (auto& screen: screens_) {
        if (screen->getName() == name) {
            return index;
        }
        index++;
    }

    return -1;
}
