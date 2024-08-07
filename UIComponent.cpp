//
// Created by Luca Schmitt on 04.08.24.
//

#include <SFML/Graphics.hpp>
#include "UIComponent.h"
#include <string>

UIComponent::UIComponent(std::string name, const sf::Vector2f &sizeProportions)
: name_(std::move(name)), sizeProportions_(sizeProportions) {
    // Assign default values
    needRenderUpdate_ = true;
    isVisible_ = true;

    // Should this be true or false? And should this be renamed to isDisabled?
    isEnabled_ = true;
}

void UIComponent::setVisible(bool visible) {
    // Only update visibility and render update flag if needed
    if (isVisible_ != visible) {
        isVisible_ = visible;
        needRenderUpdate_ = true;
    }
}

void UIComponent::setEnabled(bool enabled) {
    // Should only the inner line stay here?
    if (isEnabled_ != enabled) {
        isEnabled_ = enabled;
    }
}

void UIComponent::setParentSize(sf::Vector2f parentSize) {
    parentSize_ = parentSize;

    // Update render update flag
    needRenderUpdate_ = true;
}

void UIComponent::setSizeProportions(sf::Vector2f sizeProportions) {
    sizeProportions_ = sizeProportions;

    // Update render update flag
    needRenderUpdate_ = true;
}

void UIComponent::setPosition(sf::Vector2f position) {
    position_ = position;

    // Update render update flag
    needRenderUpdate_ = true;
}

void UIComponent::updatePosition(sf::Vector2f offset) {
    position_ += offset;

    // Update render update flag
    needRenderUpdate_ = true;
}

void UIComponent::setCallback(std::function<void()> callback) {
    callback_ = std::move(callback);
}

bool UIComponent::isVisible() const {
    return isVisible_;
}

bool UIComponent::isEnabled() const {
    return isEnabled_;
}

const std::string &UIComponent::getName() const {
    return name_;
}

const sf::Vector2f &UIComponent::getPosition() const {
    return position_;
}

const sf::Vector2f &UIComponent::getSize() const {
    return size_;
}
