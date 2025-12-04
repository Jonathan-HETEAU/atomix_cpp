#include "atom.hpp"

namespace Atomix
{

    void Atom::toSelected()
    {
        selected = true;
    }
    
    void Atom::toWait()
    {
        selected = false;
    }

    bool Atom::isSelected()
    {
        return selected;
    }
    
    bool Atom::isWaited()
    {
        return !selected;
    }

}
