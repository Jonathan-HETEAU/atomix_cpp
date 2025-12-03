#pragma once
#include <optional>
#include <set>
#include <vector>
#include "atomix.hpp"
#include "painter.hpp"

namespace Atomix{

    enum Action {

    };


    class Partie
    {
    private:
        
        std::set<Direction> movesPossible = {};
    
        Level& level;
        std::vector<Atom> atoms = {};
        std::optional<int> selected ;
        
        
        void updateAtomSelectedMoves();
        inline bool isInMap(Position &position);

    public:
        Partie(Level &level);
        ~Partie();
        virtual void draw(Painter &painter);
        void onClick(Position position);
        
    };


}