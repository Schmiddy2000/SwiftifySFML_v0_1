//
// Created by Luca Schmitt on 04.08.24.
//

/*
 * Thoughts / Improvements:
 * - Add background and outline functionalities. Should a container class have a sf::RectangleShape regardless, or
 * should a new one be created it appearance parameters are set? In this case these might have to be deleted in
 * the destructor → Use unique_ptr's for this.
 * → Should there also be a function to remove the background or can this be handled internally?
 *
 * - Add moveElement functionalities. This returns the unique_ptr and removes it from the container's elements_
 *
 * - Are unique names going to be enforced within a single container? And within nested containers?
 *
 * - Should names be moved in the construction process? This might complicate iterative naming with base names
 *
 * - Use templates for the add, get and remove functions for Containers and UIComponents
 *
 * - Add safety checks to the functions computing internal properties from the proportions. It's imperative that
 * parentSize_ != 0.
 *
 * - Should containers and UI components have an updatePosition method that takes an sf::Vector2f offset?
 *
 * Details on the implementation:
 *
 */


#include "Container.h"

#include <utility>
#include <iostream>

// ___________________________________________________________________________
Container::Container(std::string name, sf::Vector2f sizeProportions)
: name_(std::move(name)), sizeProportions_(sizeProportions) {

    // Assign default values
    layoutOrientation_ = LayoutOrientation::None;
    alignment_ = Alignment::Center;
    needRenderUpdate_ = true;
    isVisible_ = true;

    // Should this be true or false? And should this be renamed to isDisabled?
    isEnabled_ = true;
}

Container::Container(Container&& other) noexcept : needRenderUpdate_(other.needRenderUpdate_),
                                                   name_(std::move(other.name_)),
                                                   parentSize_(other.parentSize_),
                                                   sizeProportions_(other.sizeProportions_),
                                                   paddingProportions_(other.paddingProportions_),
                                                   spacingProportions_(other.spacingProportions_),
                                                   position_(other.position_),
                                                   size_(other.size_),
                                                   padding_(other.padding_),
                                                   spacing_(other.spacing_),
                                                   layoutOrientation_(other.layoutOrientation_),
                                                   alignment_(other.alignment_),
                                                   elements_(std::move(other.elements_)),
                                                   callback_(std::move(other.callback_)),
                                                   isEnabled_(other.isEnabled_),
                                                   isVisible_(other.isVisible_),
                                                   backgroundShape_(std::move(other.backgroundShape_)) {}

Container& Container::operator=(Container&& other) noexcept {
    if (this != &other) {
        needRenderUpdate_ = other.needRenderUpdate_;
        name_ = std::move(other.name_);
        parentSize_ = other.parentSize_;
        sizeProportions_ = other.sizeProportions_;
        paddingProportions_ = other.paddingProportions_;
        spacingProportions_ = other.spacingProportions_;
        position_ = other.position_;
        size_ = other.size_;
        padding_ = other.padding_;
        spacing_ = other.spacing_;
        layoutOrientation_ = other.layoutOrientation_;
        alignment_ = other.alignment_;
        elements_ = std::move(other.elements_);
        callback_ = std::move(other.callback_);
        isEnabled_ = other.isEnabled_;
        isVisible_ = other.isVisible_;
        backgroundShape_ = std::move(other.backgroundShape_);
    }
    return *this;
}

// ___________________________________________________________________________
void Container::setParentSize(sf::Vector2f parentSize) {
    parentSize_ = parentSize;
    computeSize();

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::setSizeProportions(sf::Vector2f sizeProportions) {
    // Update containers size and size proportions
    sizeProportions_ = sizeProportions;
    computeSize();

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::setPaddingProportions(sf::Vector2f paddingProportions) {
    paddingProportions_ = paddingProportions;
    computePadding();

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::setSpacingProportions(sf::Vector2f spacingProportions) {
    spacingProportions_ = spacingProportions;
    computeSpacing();

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::setPosition(sf::Vector2f position) {
    position_ = position;

    // Update render update flag
    needRenderUpdate_ = true;
}

void Container::updatePosition(sf::Vector2f offset) {
    position_ += offset;

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::setLayoutOrientation(LayoutOrientation layoutOrientation) {
    layoutOrientation_ = layoutOrientation;

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::setAlignment(Alignment alignment) {
    alignment_ = alignment;

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
// Can it cause problems when the function is being moved here?
// ___________________________________________________________________________
void Container::setCallback(std::function<void()> callback) {
    callback_ = std::move(callback);
}

// ___________________________________________________________________________
void Container::setVisible(bool visible) {
    // Only update visibility and render update flag if needed
    if (isVisible_ != visible) {
        isVisible_ = visible;
        needRenderUpdate_ = true;
    }
}

// ___________________________________________________________________________
void Container::setEnabled(bool enabled) {
    // Should only the inner line stay here?
    if (isEnabled_ != enabled) {
        isEnabled_ = enabled;
    }
}

// ___________________________________________________________________________
void Container::setOutline(sf::Color color, float thickness) {
    // Create a background shape instance if it doesn't exist yet
    if (!backgroundShape_) {
        backgroundShape_ = std::make_unique<sf::RectangleShape>();
    }

    // Set the outline attributes
    backgroundShape_->setOutlineColor(color);
    backgroundShape_->setOutlineThickness(thickness);
}

// ___________________________________________________________________________
void Container::setBackground(sf::Color color) {
    // Create a background shape instance if it doesn't exist yet
    if (!backgroundShape_) {
        backgroundShape_ = std::make_unique<sf::RectangleShape>();
    }

    // Set the background color
    backgroundShape_->setFillColor(color);
}


// ___________________________________________________________________________
const std::string &Container::getName() const {
    return name_;
}

// ___________________________________________________________________________
const sf::Vector2f &Container::getPosition() const {
    return position_;
}

// ___________________________________________________________________________
const sf::Vector2f &Container::getSize() const {
    return size_;
}

// ___________________________________________________________________________
const sf::Vector2f &Container::getPadding() const {
    return padding_;
}

// ___________________________________________________________________________
const sf::Vector2f &Container::getSpacing() const {
    return spacing_;
}

// ___________________________________________________________________________
const bool &Container::isVisible() const {
    return isVisible_;
}

// ___________________________________________________________________________
const bool &Container::isEnabled() const {
    return isEnabled_;
}

// ___________________________________________________________________________
// Could make this function templated.
// ___________________________________________________________________________
Container &Container::getContainer(const std::string &name) {
    // Counter to store the number of fits for a wrong type. Used for error messages
    size_t wrongTypeFits = 0;

    for (auto& element : elements_) {
        // Using std::visit to get the name
        auto elementName = std::visit([](auto& el) -> const std::string& { return el->getName(); }, element);

        // Check if the element's name matches
        if (elementName == name) {
            // Check if the type matches as well
            if (auto containerPtr = std::get_if<std::unique_ptr<Container>>(&element)) {
                return **containerPtr;
            } else {
                wrongTypeFits++;
            }
        }
    }

    if (wrongTypeFits > 0) {
        throw std::runtime_error("Name found but type mismatch.");
    } else {
        throw std::runtime_error("No matching name found.");
    }
}

// ___________________________________________________________________________
UIComponent &Container::getUIComponent(const std::string &name) {
    // Counter to store the number of fits for a wrong type. Used for error messages
    size_t wrongTypeFits = 0;

    for (auto& element : elements_) {
        // Using std::visit to get the name
        auto elementName = std::visit([](auto& el) -> const std::string& { return el->getName(); }, element);

        // Check if the element's name matches
        if (elementName == name) {
            // Check if the type matches as well
            if (auto containerPtr = std::get_if<std::unique_ptr<UIComponent>>(&element)) {
                return **containerPtr;
            } else {
                wrongTypeFits++;
            }
        }
    }

    if (wrongTypeFits > 0) {
        throw std::runtime_error("Name found but type mismatch.");
    } else {
        throw std::runtime_error("No matching name found.");
    }
}

// ___________________________________________________________________________
// Could also make these functions templated.
// ___________________________________________________________________________
void Container::addContainer(std::unique_ptr<Container> container) {
    // Using emplace_back and std::move to construct the container at the end of elements_
    elements_.emplace_back(std::move(container));

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
void Container::addUIComponent(std::unique_ptr<UIComponent> component) {
    // Using emplace_back and std::move to construct the container at the end of elements_
    elements_.emplace_back(std::move(component));

    // Update render update flag
    needRenderUpdate_ = true;
}

// ___________________________________________________________________________
// Assume that unique names within containers are guaranteed
// ___________________________________________________________________________
void Container::removeContainer(const std::string &name) {
    for (auto it = elements_.begin(); it != elements_.end(); ++it) {
        // Use std::visit to get the name of the element
        auto elementName = std::visit([](auto& el) -> const std::string& { return el->getName(); }, *it);

        // Check if the element's name matches the given name
        if (elementName == name) {
            // Check if the element is of type std::unique_ptr<Container>
            if (std::holds_alternative<std::unique_ptr<Container>>(*it)) {
                // Remove the element
                elements_.erase(it);

                // Update render update flag
                needRenderUpdate_ = true;

                // Exit the function after removing the element
                return;
            } else {
                // Here the unique name assumption is made
                throw std::runtime_error("Name found but type mismatch.");
            }
        }
    }
    // If no matching name is found
    throw std::runtime_error("No matching name found.");
}

// ___________________________________________________________________________
void Container::removeUIComponent(const std::string &name) {
    for (auto it = elements_.begin(); it != elements_.end(); ++it) {
        // Use std::visit to get the name of the element
        auto elementName = std::visit([](auto& el) -> const std::string& { return el->getName(); }, *it);

        // Check if the element's name matches the given name
        if (elementName == name) {
            // Check if the element is of type std::unique_ptr<Container>
            if (std::holds_alternative<std::unique_ptr<UIComponent>>(*it)) {
                // Remove the element
                elements_.erase(it);

                // Update render update flag
                needRenderUpdate_ = true;

                // Exit the function after removing the element
                return;
            } else {
                // Here the unique name assumption is made
                throw std::runtime_error("Name found but type mismatch.");
            }
        }
    }
    // If no matching name is found
    throw std::runtime_error("No matching name found.");
}

// ___________________________________________________________________________
void Container::draw(sf::RenderWindow &window) {
    // Check for render updates using the render update flag
    if (needRenderUpdate_) {
        computeRenderInformation();
    }

    // Check for background and outline drawing instructions and draw them if applicable. Could add additional
    // checks here like outlineColor == backgroundColor, outlineThickness == 0, and more.
    if (backgroundShape_ and isVisible_) {
        window.draw(*backgroundShape_);
    }

    // Loop over all elements and call their respective draw functions
    for (auto& element : elements_) {
        std::visit([&window](auto& el) { el->draw(window); }, element);
    }
}

// ___________________________________________________________________________
// Think of a smarter way and also how cases of, for example, overlapping should be handled. Is there a way to only
// loop though the elements that have a callback set?

// Could add a feature that checks, for example, in the compute render information function, how many callback
// components are within a certain container. This could lead to fewer checks necessary in the handleEvent
// function-cascade

// How events should be handled in a container:
// - Events should propagate through the hierarchy of containers from the top to the bottom
// - The most specific element should be triggered. Use a flag to indicate if an event has been handled
// ___________________________________________________________________________
bool Container::handleEvent(sf::Event &event) {
    bool hasBeenHandled = false;
    // Loop over all other elements and call their handleEvent functions
    for (auto& element: elements_) {
        hasBeenHandled = std::visit([&event](auto& el) -> bool { return el->handleEvent(event); }, element);

        // Exit the loop if the
        if (hasBeenHandled) break;
    }
    // If any of them returns true, indicating that the event has been handled on a lower level, handle the own
    // container event if applicable
    if (not hasBeenHandled and callback_) {
        callback_();
    }

    // Return this container's handle status
    return hasBeenHandled;
}

// ___________________________________________________________________________
void Container::computeSize() {
    // Compute the actual size. Needs parentSize to be set already.
    size_.x = sizeProportions_.x * parentSize_.x;
    size_.y = sizeProportions_.y * parentSize_.y;

    // Update the parentSize of all child elements
    for (auto& element: elements_) {
        std::visit([this] (auto& el) { el->setParentSize(size_); }, element);
    }
}

// ___________________________________________________________________________
void Container::computePadding() {
    // Compute padding. Needs the size to be set already.
    padding_.x = paddingProportions_.x * size_.x;
    padding_.y = paddingProportions_.y * size_.y;
}

// ___________________________________________________________________________
void Container::computeSpacing() {
    // Compute the spacing. Needs the size to be set already.
    spacing_.x = spacingProportions_.x * size_.x;
    spacing_.y = spacingProportions_.y * size_.y;
}

// ___________________________________________________________________________
// List of checks that should be done:
// - Has the parentSize been set, is it different from sf::Vector(0, 0)
// - Check that the sum of the nested size proportions including padding and spacing doesn't exceed 1. If that is
// the case, print out a warning and scale the proportions down to fit in the container
// ___________________________________________________________________________
void Container::validateContainerContent() {
    // Check if the size is greater than 0 in both x- and y-direction
    if (size_.x <= 0 or size_.y <= 0) {
        std::cerr << "Container '" << this->name_ << "' has faulty size: (" << size_.x << ", " << size_.y << ")" << std::endl;
    }

    // Check the sum of size proportions only when there's at least one element in the container
    if (!elements_.empty()) {
        // Store the total sum of proportions
        sf::Vector2f proportionSizeInContainer;

        // Loop though all elements and add the sizeProportions

        // Do a check if the limit isn't exceeded yet

        // Add the proportions caused by spacing and check if the limit still isn't exceeded

        // Take actions accordingly
    }


}

/*
 * Observations:
 * --- Horizontal layout ---
 * - Top alignments work as intended
 * - Center alignments seem to be a bit too low. Doesn't seem to be affected by padding
 * - Bottom alignment is way too low
 * - Alignment along the secondary axis seems to work as intended
 *
 * --- Vertical layout ---
 * - The primary axis works fine for Top alignment. For lower configurations, the elements are moved too far to
 * the bottom
 * - The secondary axis works for the Leading alignment, but for the others, it is way less affected than intended
 *
 * --- Stacked layout ---
 * - The program simply crashes (interrupted by signal 11: SIGSEGV)
 */

// ___________________________________________________________________________
// Function logic walk through:
// - Use the top left corner as an anchor point for the first element
// - Use a switch statement to distinguish between the different layout cases
// - Place elements one after another with the specified spacing amount in between. Only use the layouts primary
// axis for spacing. All elements align with the container wall regarding the secondary axis.
// For the 'None' and 'Stacked' layout orientation, all elements align their positions with the top left corner.
//
// This could likely be improved by calculating the positions in separate functions and returning arrays. Only in
// the end, the positions should actually be updated.
// ___________________________________________________________________________
void Container::handleLayoutOrientation() {
    // Variable to store the position, size, and count the current element
    sf::Vector2f currentPosition = position_;
    sf::Vector2f elementSize;
    size_t elementCounter = 0;


    for (const auto& element: elements_) {
        // Increment the elementCounter
        elementCounter++;
        // Set the element's position to the current position
        std::visit([&currentPosition] (auto& el) { el->setPosition(currentPosition); }, element);

        // Switch layout orientations and update the current position accordingly
        switch (layoutOrientation_) {

            case LayoutOrientation::Horizontal:
                // Increment the x component of the current position by the element's size along this axis
                elementSize = std::visit([] (auto& el) -> sf::Vector2f { return el->getSize(); }, element);
                currentPosition.x += elementSize.x;

                // Increment the x component of the current position by the horizontal axis value of 'spacing'
                currentPosition.x += spacing_.x;
                break;

            case LayoutOrientation::Vertical:
                // Increment the y component of the current position by the element's size along this axis
                elementSize = std::visit([] (auto& el) -> sf::Vector2f { return el->getSize(); }, element);
                currentPosition.y += elementSize.y;

                // Increment the x component of the current position by the vertical axis value of 'spacing'
                currentPosition.y += spacing_.y;
                break;

            case LayoutOrientation::None:
                // Technically, this only needs to be printed once and also only if the container contains two or
                // more elements
                if (elementCounter == 2) {
                    std::cout << "Warning, layout orientation in container " << this->name_ << " is 'None'" << std::endl;
                }
            case LayoutOrientation::Stacked:
                break;
        }
    }
}

// ___________________________________________________________________________
/*
 * Function logic walk-through:
 * - Compute the available space along the horizontal axis, excluding padding. For this, use padding, position of
 * the first element and size and position of the last element
 * - Align the bulk of spaced elements on the primary, here horizontal, axis
 * - Align each element separately on the secondary, here vertical, axis
 *
 * 'Center' should be the default
 */
// ___________________________________________________________________________
void Container::handleHorizontalAlignment() {
    // Variable to reference the bottom right corner, accounting for padding
    sf::Vector2f adjustedBottomRightCorner = position_ + size_ - padding_;

    // Variable to store the primary offset, here applied in the horizontal direction
    float primaryOffset = padding_.x;

    // Store the secondary base offset, from the perspective of the largest element inside the container. In the switch
    // implementation, tweaks are being calculated and added for the 'centered' and 'bottom' cases in the final
    // assignment loop.
    float secondaryBaseOffset = padding_.y;

    // Using the last element is kind of none-nonsensical here. It should take the last element along the primary
    // axis and the largest element along the secondary axis

    // Get the position and size of the last element in the elements_ array
    sf::Vector2f lastElementPosition = std::visit([] (auto& el)
            -> sf::Vector2f { return el->getPosition(); }, elements_.back());
    sf::Vector2f lastElementSize = std::visit([] (auto& el)
            -> sf::Vector2f { return el->getSize();}, elements_.back());

    // Optimized approach to get the largest element. Also enables to use the size data later
    std::vector<float> secondarySizes;
    for (const auto& element: elements_) {
        sf::Vector2f currentElementSize = std::visit([](auto& el) -> sf::Vector2f { return el->getSize(); }, element);

        secondarySizes.push_back(currentElementSize.y);
    }

    // Get the largest size along the secondary axis
    float largestSecondarySize = *std::max_element(secondarySizes.begin(), secondarySizes.end());

    // Use a 'theoretically largest element' to fix the alignment issues
    sf::Vector2f theoreticallyLargestElement(lastElementSize.x, largestSecondarySize);

    // The alignmentBuffer saves the amount of space that isn't taken up by any elements in each direction.
    // sf::Vector2f alignmentBuffer = adjustedBottomRightCorner - lastElementPosition - lastElementSize - padding_;
    sf::Vector2f alignmentBuffer = adjustedBottomRightCorner - lastElementPosition - theoreticallyLargestElement - padding_;

    // Switch alignment cases
    switch (alignment_) {

        // Showcase of previous approach
//        case Alignment::TopLeading:
//            // Do nothing since this is the default
//            break;
//        case Alignment::Top:
//            primaryOffset += alignmentBuffer.x / 2;
//            break;
//        case Alignment::TopTrailing:
//            primaryOffset += alignmentBuffer.x;
//            break;

        // Alternative, code-wise more compact approach
        case Alignment::TopTrailing:
            primaryOffset += alignmentBuffer.x / 2;
        case Alignment::Top:
            primaryOffset += alignmentBuffer.x / 2;
        case Alignment::TopLeading:
            // Reset all array entries to 0 since no additional secondary offset is necessary. Definitely suboptimal
            std::transform(secondarySizes.begin(), secondarySizes.end(), secondarySizes.begin(),
                           [](float y) { return 0; });
            break;

        case Alignment::Trailing:
            primaryOffset += alignmentBuffer.x / 2;
        case Alignment::Center:
            primaryOffset += alignmentBuffer.x / 2;
        case Alignment::Leading:
            // Update the secondary buffer using the alignment buffer. This update is in respect to the largest element
            secondaryBaseOffset += alignmentBuffer.y / 2;

            // Transform the secondarySizes array into an array of unique offsets. This essentially centers the
            // middle axis of the elements along the middle axis of the largest element.
            std::transform(secondarySizes.begin(), secondarySizes.end(), secondarySizes.begin(),
                           [largestSecondarySize](float y) { return (largestSecondarySize - y) / 2; });
            break;


        case Alignment::BottomTrailing:
            primaryOffset += alignmentBuffer.x / 2;
        case Alignment::Bottom:
            primaryOffset += alignmentBuffer.x / 2;
        case Alignment::BottomLeading:
            // Update the secondary buffer using the alignment buffer. This update is in respect to the largest element
            secondaryBaseOffset += alignmentBuffer.y;

            // Transform the secondarySizes array into an array of unique offsets. This essentially aligns the
            // elements along the bottom axis of the largest element.
            std::transform(secondarySizes.begin(), secondarySizes.end(), secondarySizes.begin(),
                           [largestSecondarySize](float y) { return largestSecondarySize - y; });
            break;
    }

    // Apply the actual position transform by looping over all elements
    size_t indexCounter = 0;
    for (auto& element: elements_) {
        // Calculate the final offset for each element and update the position accordingly
        sf::Vector2f offset(primaryOffset, secondaryBaseOffset + secondarySizes[indexCounter]);
        std::visit([&offset](auto& el) { el->updatePosition(offset); }, element);

        // Increment the index counter
        indexCounter++;
    }
}

// ___________________________________________________________________________
void Container::handleVerticalAlignment() {
    // Variable to reference the bottom right corner, accounting for padding
    sf::Vector2f adjustedBottomRightCorner = position_ + size_ - padding_;

    // Variable to store the primary offset, here applied in the vertical direction
    float primaryOffset = padding_.y;

    // Store the secondary base offset, from the perspective of the largest element inside the container.
    float secondaryBaseOffset = padding_.x;

    // Get the position and size of the last element in the elements_ array
    sf::Vector2f lastElementPosition = std::visit([](auto& el) -> sf::Vector2f { return el->getPosition(); }, elements_.back());
    sf::Vector2f lastElementSize = std::visit([](auto& el) -> sf::Vector2f { return el->getSize(); }, elements_.back());

    // Collect the secondary sizes (widths in this case)
    std::vector<float> secondarySizes;
    for (const auto& element : elements_) {
        sf::Vector2f currentElementSize = std::visit([](auto& el) -> sf::Vector2f { return el->getSize(); }, element);
        secondarySizes.push_back(currentElementSize.x);
    }

    // Get the largest size along the secondary axis
    float largestSecondarySize = *std::max_element(secondarySizes.begin(), secondarySizes.end());

    // Use a 'theoretically largest element' to fix the alignment issues
    sf::Vector2f theoreticallyLargestElement(largestSecondarySize, lastElementSize.y);

    // The alignmentBuffer saves the amount of space that isn't taken up by any elements in each direction.
    sf::Vector2f alignmentBuffer = adjustedBottomRightCorner - lastElementPosition - theoreticallyLargestElement - padding_;

    switch (alignment_) {
        case Alignment::BottomLeading:
            primaryOffset += alignmentBuffer.y / 2;
            // Fallthrough intentional?
        case Alignment::Leading:
            primaryOffset += alignmentBuffer.y / 2;
            // Fallthrough intentional?
        case Alignment::TopLeading:
            // Reset all array entries to 0 since no additional secondary offset is necessary. Definitely suboptimal
            std::transform(secondarySizes.begin(), secondarySizes.end(), secondarySizes.begin(),
                           [](float x) { return 0; });
            break;

        case Alignment::Bottom:
            primaryOffset += alignmentBuffer.y / 2;
            // Fallthrough intentional?
        case Alignment::Center:
            primaryOffset += alignmentBuffer.y / 2;
            // Fallthrough intentional?
        case Alignment::Top:
            // Center elements along the horizontal axis
            secondaryBaseOffset += alignmentBuffer.x / 2;
            std::transform(secondarySizes.begin(), secondarySizes.end(), secondarySizes.begin(),
                           [largestSecondarySize](float x) { return (largestSecondarySize - x) / 2; });
            break;

        case Alignment::BottomTrailing:
            primaryOffset += alignmentBuffer.y / 2;
            // Fallthrough intentional?
        case Alignment::Trailing:
            primaryOffset += alignmentBuffer.y / 2;
            // Fallthrough intentional?
        case Alignment::TopTrailing:
            // Align elements along the bottom of the largest element
            secondaryBaseOffset += alignmentBuffer.x;
            std::transform(secondarySizes.begin(), secondarySizes.end(), secondarySizes.begin(),
                           [largestSecondarySize](float x) { return largestSecondarySize - x; });
            break;
    }

    // Apply the actual position transform by looping over all elements
    size_t indexCounter = 0;
    for (auto& element : elements_) {
        // Calculate the final offset for each element and update the position accordingly
        sf::Vector2f offset(secondaryBaseOffset + secondarySizes[indexCounter], primaryOffset);
        std::visit([&offset](auto& el) { el->updatePosition(offset); }, element);

        // Increment the index counter
        indexCounter++;
    }
}

// ___________________________________________________________________________
/*
 * Function logic walk-through:
 * - Elements should be placed individually along both axes
 */
// ___________________________________________________________________________
void Container::handleStackedAlignment() {
    // Reduced size of the container, excluding padding
    sf::Vector2f reducedSize = position_ + size_ - padding_ - padding_;

    // Get the sizes to then calculate the offsets in the switch statement
    std::vector<sf::Vector2f> elementSizes;

    // Get the sizes and add them to elementSizes
    for (const auto& element: elements_) {
        // Compute the element's size and add it to the back of elementSizes
        sf::Vector2f currentElementsSize = std::visit([] (auto& el) -> sf::Vector2f { return el->getSize(); }, element);
        elementSizes.push_back(currentElementsSize);
    }

    // Variable alignment buffers
    std::vector<sf::Vector2f> alignmentBuffers(elements_.size());
    std::transform(elementSizes.begin(), elementSizes.end(), alignmentBuffers.begin(),
                   [&reducedSize] (sf::Vector2f elementSize) { return reducedSize - elementSize; });

    sf::Vector2f alignmentBufferFactor(0, 0);

    // Here an offset vector should do the trick
    std::vector<sf::Vector2f> elementOffsets(elements_.size());


    switch (alignment_) {

        case Alignment::TopTrailing:
            alignmentBufferFactor.x += 1.0 / 2;
        case Alignment::Top:
            alignmentBufferFactor.x += 1.0 / 2;
        case Alignment::TopLeading:
            break;

        case Alignment::Trailing:
            alignmentBufferFactor.x += 1.0 / 2;
        case Alignment::Center:
            alignmentBufferFactor.x += 1.0 / 2;
        case Alignment::Leading:
            // Factor to center all elements around the horizontal middle axis
            alignmentBufferFactor.y += 1.0 / 2;
            break;


        case Alignment::BottomTrailing:
            alignmentBufferFactor.x += 1.0 / 2;
        case Alignment::Bottom:
            alignmentBufferFactor.x += 1.0 / 2;
        case Alignment::BottomLeading:
            // Factor to center all elements around the horizontal middle axis
            alignmentBufferFactor.y += 1;
            break;
    }

    // Apply the position update by looping over all elements
    size_t indexCounter = 0;
    for (auto& element: elements_) {
        // Compute the update for each element and update the positions accordingly
        sf::Vector2f offset(padding_ + sf::Vector2f(alignmentBufferFactor.x * alignmentBuffers[indexCounter].x,
                                                    alignmentBufferFactor.y * alignmentBuffers[indexCounter].y));
        std::visit([&offset] (auto& el) { el->updatePosition(offset); }, element);

        // Increment the index counter
        indexCounter++;
    }
}

// ___________________________________________________________________________
void Container::computeRenderInformation() {
    // Check the container content here. Raises errors if necessary
    validateContainerContent();

    // Also handle the possible backgroundShape here (like setting position and size)
    if (backgroundShape_) {
        // Update the parameters that can be changed here and won't be handled in other functions
        backgroundShape_->setPosition(position_);
        backgroundShape_->setSize(size_);
    }

    // Handle the layout and alignment here
    handleLayoutOrientation();

    // This could also be moved to the handleLayout function since there's a switch already present anyway
    switch (layoutOrientation_) {

        case LayoutOrientation::Horizontal:
            handleHorizontalAlignment();
            break;
        case LayoutOrientation::Vertical:
            handleVerticalAlignment();
            break;
        case LayoutOrientation::None:
            // Potentially add error messages / warnings here
        case LayoutOrientation::Stacked:
            handleStackedAlignment();
            break;
    }

    // Reset the render update flag
    needRenderUpdate_ = false;
}
