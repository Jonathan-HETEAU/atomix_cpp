#pragma once
#include "atomix.hpp"

namespace Atomix
{

    class Painter
    {

    public:
        Painter() = default;
        virtual ~Painter() = default;  // Virtual destructor for polymorphism
        
        // Pure virtual functions (must be implemented by derived classes)
        virtual void drawAtom(Atom &atom) = 0;
        virtual void drawMap(int width, int height, bool **map) = 0;
    };

}
