#include "partie.hpp"

namespace Atomix
{
    Partie::Partie(LevelData &level) : level(level)
    {
        for (int indexAtom = 0; indexAtom < level.nbrAtoms; indexAtom++)
        {
            atoms.push_back(Atom(indexAtom,level.atoms[indexAtom]));
        }
    }

    void Partie::draw(Painter &painter)
    {
        painter.drawMap(level.width, level.height, level.map);
        for (auto &atom : atoms)
        {
            painter.drawAtom(atom);
        }
    }

    void Partie::onClick(Position position)
    {
        if (!isInMap(position))
            return;

        if (selected.has_value())
        {
            Atom &atomSelected = atoms[selected.value()];
            if (atomSelected == position)
                return;

            for (Direction direction : atomSelected.movePosible)
            {
                if (position == (atomSelected + direction))
                {
                    moveAtom(atomSelected, direction);
                    return;
                }
            }
            atomSelected.toWait();
            DispatchOnAtomUnselected(atomSelected);
            selected.reset();
        }

        if (!selected.has_value())
        {
            for (int atomIndex = 0; atomIndex < atoms.size(); atomIndex++)
            {
                if (atoms[atomIndex].data.position == position)
                {
                    selected.emplace(atomIndex);
                    atoms[atomIndex].toSelected();
                    updateAtomMoves(atoms[atomIndex]);
                    DispatchOnAtomSelected(atoms[atomIndex]);
                }
            }
        }
    }

    void Partie::onSwitchAtom()
    {

        if (selected.has_value())
        {
            atoms[selected.value()].toWait();
            DispatchOnAtomUnselected(atoms[selected.value()]);
            selected.emplace((selected.value() + 1) % atoms.size());
        }
        else
        {
            selected.emplace(0);
        }
        atoms[selected.value()].toSelected();
        updateAtomMoves(atoms[selected.value()]);
        DispatchOnAtomSelected(atoms[selected.value()]);
    }

    void Partie::onMoveAtom(Direction direction)
    {
        if (selected.has_value())
        {
            moveAtom(atoms[selected.value()], direction);
        }
    }

    inline bool Partie::isInMap(Position &position)
    {
        return position.x >= 0 &&
               position.y >= 0 &&
               position.x < level.width &&
               position.y < level.height;
    }

    void Partie::moveAtom(Atom &atom, Direction direction)
    {
        updateAtomMoves(atom);
        Position origin = atom.data.position;
        bool hasChange = false;
        while (atom.movePosible.contains(direction))
        {
            atom += direction;
            updateAtomMoves(atom);
            hasChange = true;
        }
        if (hasChange)
        {
            DispatchOnAtomMove(atom, origin, atom.data.position);
        }
        checkIsWin();
    }

    void Partie::updateAtomMoves(Atom &atom)
    {
        atom.movePosible.clear();
        for (auto direction : DIRECTION_VECTOR)
        {
            Position position = (atom.data.position + direction.second);
            if (level.map[position.y][position.x])
            {
                bool isempty = true;
                for (auto atom : atoms)
                {
                    if (atom.data.position == position)
                    {
                        isempty = false;
                        break;
                    }
                }
                if (isempty)
                    atom.movePosible.emplace(direction.first);
            }
        }
    }

    void Partie::checkIsWin()
    {
        std::map<Position, Atom &> mapping = {};
        for (Atom &atom : atoms)
        {
            mapping.insert({atom.data.position, atom});
        }

        this->win = false;
        AtomData &atomRacine = level.solution[0];
        for (Atom &atom : atoms)
        {
            if (atom == atomRacine.value)
            {
                bool win = true;
                for (int s = 1; s < level.nbrAtoms; s++)
                {
                    Position nextAtomPositionInPartie = atom + (level.solution[s].position - atomRacine.position);
                    if (mapping.contains(nextAtomPositionInPartie) && mapping.at(nextAtomPositionInPartie) == level.solution[s].value)
                    {
                        continue;
                    }
                    win = false;
                    break;
                }
                if (win)
                {
                    this->win = true;
                    DispatchOnWin();
                    return;
                }
            }
        }
    }

    void Partie::DispatchOnWin()
    {
        for (auto observer : observers)
        {
            observer->onWin();
        }
    }
    void Partie::DispatchOnAtomSelected(Atom &selected)
    {
        for (auto observer : observers)
        {
            observer->onAtomSelected(selected);
        }
    }
    void Partie::DispatchOnAtomUnselected(Atom &unselected)
    {
        for (auto observer : observers)
        {
            observer->onAtomUnselected(unselected);
        }
    }
    void Partie::DispatchOnAtomMove(Atom &atom, Position &origin, Position &dest)
    {
        for (auto observer : observers)
        {
            observer->onAtomMove(atom, origin, dest);
        }
    }

    void Partie::addObserver(PartieObserver & observer){
        observers.emplace(&observer);
    }
    void Partie::removeObserver(PartieObserver & observer){
        observers.erase(&observer);
    }

    bool Partie::isWin()
    {
        return win;
    }

    Partie::~Partie()
    {
    }

}
