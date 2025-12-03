#pragma once
#include "atomix.hpp"

namespace Atomix
{

    enum AtomState
    {
        Selected,
        Move,
        Wait,
    };

    class Atom
    {

    private:
        AtomState state;

    public:
        AtomData &data;

        Atom(AtomData &data) : data(data), state(Wait) {};
        ~Atom() = default;

        void toSelected();
        void toMove();
        void toWait();

        bool isSelected();
        bool isMoved();
        bool isWaited();

        AtomState getState();
        void setState(AtomState state);
    };

}