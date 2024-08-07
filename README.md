# SwiftifySFML

Framework for dynamic layout handling build on top of SFML (Simple Fast Media Library) in C++. Inspired by Swift and allows container-based UI construction. Still in its infancy and not as functional or flexible yet, as intended.

## Idea

1. Create an `Application`
2. Create `Screens` and add them to the `Application`
3. Create `Containers` and add them to the `Screens`
4. Create nested structures by adding `UIComponents` and `Containers`

Screens and Containers feature layout orientations, alignment cases and spacing to positions the nested components. The positions are then calculated from sizeProportions specified in the constructors.
