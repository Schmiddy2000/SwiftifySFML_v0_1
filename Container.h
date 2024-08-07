//
// Created by Luca Schmitt on 26.07.24.
//

#ifndef SWIFTIFYSFML_V0_1_CONTAINER_H
#define SWIFTIFYSFML_V0_1_CONTAINER_H

#include <SFML/Graphics.hpp>
#include "Enumerations.h"
#include "UIComponent.h"

/*
 * Container class. Can store and handle interactions and layout of multiple other Containers or UIComponents.
 */
class Container {
public:
    // Constructor and destructor
    Container(std::string name, sf::Vector2f sizeProportions);
    ~Container() = default;

    // Delete copy constructor and copy assignment operator
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;

    // Move constructor and move assignment operator
    Container(Container&& other) noexcept;
    Container& operator=(Container&& other) noexcept;

    // Setter functions for the non-computed properties
    void setParentSize(sf::Vector2f parentSize);

    void setSizeProportions(sf::Vector2f sizeProportions);
    void setPaddingProportions(sf::Vector2f paddingProportions);
    void setSpacingProportions(sf::Vector2f spacingProportions);

    void setPosition(sf::Vector2f position);
    void updatePosition(sf::Vector2f offset);

    void setLayoutOrientation(LayoutOrientation layoutOrientation);
    void setAlignment(Alignment alignment);

    void setCallback(std::function<void()> callback);

    void setVisible(bool visible);
    void setEnabled(bool enabled);

    void setOutline(sf::Color color, float thickness);
    void setBackground(sf::Color color);

    // Getter functions for the non-internal properties
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const sf::Vector2f& getPosition() const;
    [[nodiscard]] const sf::Vector2f& getSize() const;
    [[nodiscard]] const sf::Vector2f& getPadding() const;
    [[nodiscard]] const sf::Vector2f& getSpacing() const;

    [[nodiscard]] const bool& isVisible() const;
    [[nodiscard]] const bool& isEnabled() const;

    // Get a reference to an element specified by its name
    Container& getContainer(const std::string& name);
    UIComponent& getUIComponent(const std::string& name);

    // Functions to add and remove elements. Add functions use unique pointers. Remove functions use the elements name
    void addContainer(std::unique_ptr<Container> container);
    void addUIComponent(std::unique_ptr<UIComponent> component);

    void removeContainer(const std::string& name);
    void removeUIComponent(const std::string& name);

    // Draw the render information to the screen
    void draw(sf::RenderWindow& window);

    // Handle events by propagating them to the lower nested levels. Returns true, if the event has been handled
    // at a lower level, else returns false. If no event has been handled, it handles the event with the set
    // callback, if applicable
    bool handleEvent(sf::Event& event);

private:
    // --- Private functions ---

    // Functions to compute the internal dimensions from the parent's dimensions and proportions
    void computeSize();
    void computePadding();
    void computeSpacing();

    // Checks the container content. Provides user feedback in the case of errors or inconsistencies.
    void validateContainerContent();

    // This function is only being called during construction and if any
    // relevant layout information changes.
    void computeRenderInformation();

    // Here go the layout and alignment handlers
    // Handle the position updates along the specified orientation axis
    void handleLayoutOrientation();

    // Handle the position offset due to the specified alignment case and the offset on the secondary orientation axis
    void handleHorizontalAlignment();
    void handleVerticalAlignment();
    void handleStackedAlignment();

    // --- Private variables ---

    // Flag indicating if the render information needs to be updated. Only set internally.
    bool needRenderUpdate_;

    // Name for identification. Probably only used in events and interactions. Set in the constructor and can only be
    // gotten afterward.
    std::string name_;

    // Absolute size of the parent container. Calculated internally
    sf::Vector2f parentSize_;

    // Proportions to calculate the specific container dimensions. Can be set via setter functions.
    sf::Vector2f sizeProportions_;
    sf::Vector2f paddingProportions_;
    sf::Vector2f spacingProportions_;

    // Position can be set by the parent container and be gotten as a const reference
    sf::Vector2f position_;

    // From the proportions computed properties to store the container's dimensions. Therefore, these can only be
    // gotten and not set directly.
    sf::Vector2f size_;
    sf::Vector2f padding_;
    sf::Vector2f spacing_;

    // Variables that affect the positioning of the nested elements. Can only be set.
    LayoutOrientation layoutOrientation_;
    Alignment alignment_;

    // Define a new type name and a variable to store nested containers and UI components in the same vector. This is
    // important because the order of adding components affects the layout.
    using ElementType = std::variant<std::unique_ptr<Container>, std::unique_ptr<UIComponent>>;
    std::vector<ElementType> elements_;

    // Stores the callback function that should be executed when an event is recognized.
    std::function<void()> callback_;

    // Booleans to handle visibility and responsiveness. Only used internally
    bool isEnabled_;
    bool isVisible_;

    // Optional sf::RectangleShape instance. Can display a background
    std::unique_ptr<sf::RectangleShape> backgroundShape_ = nullptr;
};

#endif //SWIFTIFYSFML_V0_1_CONTAINER_H
