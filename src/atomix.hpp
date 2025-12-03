#pragma once
#include <list>
#include <map>


namespace Atomix
{
    struct Position
    {
        int x;
        int y;

        bool operator==(const Position &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Position &other) const
        {
            return !(*this == other);
        }

        Position operator+(const Position &other) const
        {
            return {x + other.x, y + other.y};
        }
    };

    struct Atom
    {
        int value;
        Position position;
    };

    struct Level
    {
        int width;
        int height;
        int nbrAtoms;
        Atom *atoms;
        Atom *solution;
        bool **map;
    };

    enum Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT,
    };

    inline const std::map<Direction, Position> DIRECTION_VECTOR = {
        {UP, Position{0, -1}}, 
        {RIGHT, Position{1, 0}}, 
        {DOWN, Position{0, 1}}, 
        {LEFT, Position{-1, 0}}
    };
}