#pragma once
#include <optional>
#include <set>
#include <vector>
#include "atomix.hpp"
#include "painter.hpp"
#include "atom.hpp"

namespace Atomix{

    enum Action {

    };


    class Partie
    {
    private:
        
        LevelData& level;
        std::vector<Atom> atoms = {};
        std::optional<int> selected ;
        bool win = false;
        
        void moveAtom(Atom & atom , Direction direction);
        void updateAtomMoves(Atom & atom);
        inline bool isInMap(Position &position);
        void checkIsWin();

    public:
        Partie(LevelData &level);
        ~Partie();
        void draw(Painter &painter);
        void onClick(Position position);
        void onSwitchAtom();
        void onMoveAtom(Direction direction);
        bool isWin();
        
    };


}