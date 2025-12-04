#pragma once
#include "atomix.hpp"
#include <set>

namespace Atomix
{

    class Atom
    {

    private:
    public:
        const int id;
        AtomData data;
        bool selected;
        std::set<Direction> movePosible = {};

        Atom(int id,AtomData data) :id(id), data(data), selected(false) {};
        ~Atom() = default;
        void toSelected();
        void toWait();
        bool isSelected();
        bool isWaited();
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
            return position + data.position;
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