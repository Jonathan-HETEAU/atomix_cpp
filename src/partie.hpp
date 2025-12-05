#pragma once
#include <optional>
#include <set>
#include <vector>
#include "atomix.hpp"
#include "painter.hpp"
#include "atom.hpp"

namespace Atomix{

    class PartieObserver{
        
        public:
        PartieObserver()=default;
        ~PartieObserver()=default;
        virtual void onWin()=0;
        virtual void onAtomSelected(Atom &selected)=0;
        virtual void onAtomUnselected(Atom &unselected)=0;
        virtual void onAtomMove(Atom &atom , Position &origin , Position &dest)=0;

    };

    struct Move{
        Position origin;
        Position destination;
    };

    struct Stat {
        std::list<Move> moves;
    };

    class Partie
    {
    private:
        
        LevelData& level;
        std::vector<Atom> atoms = {};
        std::optional<int> selected ;
        bool win = false;
        
        Stat stat = {{}};
        std::set<PartieObserver *> observers = {};
        
        void DispatchOnWin();
        void DispatchOnAtomSelected(Atom &selected);
        void DispatchOnAtomUnselected(Atom &unselected);
        void DispatchOnAtomMove(Atom &atom , Position &origin , Position &dest);
        
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
        void onUndo();

        void addObserver(PartieObserver & observer);
        void removeObserver(PartieObserver & observer);
      
        LevelData& getLevel();
        Stat& getStat();
        const std::vector<Atom>& getAtoms();

        bool isWin();
        
    };


}