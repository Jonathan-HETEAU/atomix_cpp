#include "atom.hpp"

namespace Atomix
{

    void Atom::toSelected()
    {
        state = Selected;
    }
    void Atom::toMove()
    {
        state = Move;
    }
    void Atom::toWait()
    {
        state = Wait;
    }

    bool Atom::isSelected()
    {
        return state == Selected;
    }
    bool Atom::isMoved()
    {
        return state == Move;
    }
    bool Atom::isWaited()
    {
        return state == Wait;
    }

    AtomState Atom::getState()
    {
        return state;
    }

    void Atom::setState(AtomState newState)
    {
        state = newState;
    }

}
