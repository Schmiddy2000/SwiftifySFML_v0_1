//
// Created by Luca Schmitt on 26.07.24.
//

#ifndef SWIFTIFYSFML_V0_1_ENUMERATIONS_H
#define SWIFTIFYSFML_V0_1_ENUMERATIONS_H

/*
 * Alignment enum class. Contains the alignment cases that can be used for the positioning of UI elements. Can be
 * used with switch statements in the position calculations.
 *
 * Should there be a general default? Probably yes. Then it should probably be 'Center', right?
 */
enum class Alignment {
    Top,
    Bottom,
    Leading,
    Center,
    Trailing,

    // Optional additional cases
    TopLeading,
    TopTrailing,
    BottomLeading,
    BottomTrailing
};


/*
 * LayoutOrientation enum class. Alignment cases for the elements inside a container. Supports horizontal, vertical,
 * and stacked.
 */
enum class LayoutOrientation {
    Horizontal,
    Vertical,
    Stacked,

    // Should there be a default?
    None
};


/*
 * PaddingOrientation enum class. Padding cases, specifying which bounds to apply padding to.
 */
enum class PaddingOrientation {
    Top,
    Bottom,
    Leading,
    Trailing,

    // This is the default. Should there be a case for that?
    All
};

#endif //SWIFTIFYSFML_V0_1_ENUMERATIONS_H
