#pragma once
#include "atomix.hpp"
#include <set>

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
        AtomData data;
        std::set<Direction> movePosible = {};

        Atom(AtomData data) : data(data), state(Wait) {};
        ~Atom() = default;

        void toSelected();
        void toMove();
        void toWait();

        bool isSelected();
        bool isMoved();
        bool isWaited();

        AtomState getState();
        void setState(AtomState state);

        bool operator==(Position &position)
        {
            return data.position == position;
        }

        bool operator==(int &value)
        {
            return data.value == value;
        }

        Position operator+(Position &position)
        {
            return data.position + position;
        }

        Position operator+(Position position)
        {
            return position += data.position;
        }

        Position operator+(Direction &direction)
        {
            return data.position + DIRECTION_VECTOR.at(direction);
        }

        Atom &operator+=(Direction &direction)
        {
            data.position += DIRECTION_VECTOR.at(direction);
            return *this;
        }
    };

}