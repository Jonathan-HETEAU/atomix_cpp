#include "partie.hpp"

namespace Atomix
{
    Partie::Partie(LevelData &level) : level(level)
    {
        for (int indexAtom = 0; indexAtom < level.nbrAtoms; indexAtom++)
        {
            atoms.push_back(Atom(level.atoms[indexAtom]));
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
        
        if(selected.has_value()){
            Atom & atomSelected = atoms[selected.value()];
            if(atomSelected.data.position != position){
                atomSelected.toWait();
                atomSelected.movePosible.clear();
                selected.reset();

            }else{
                return;
            }
        }

        if(!selected.has_value()){
            for(int atomIndex = 0 ; atomIndex < atoms.size() ; atomIndex ++){
                if(atoms[atomIndex].data.position == position){
                    selected.emplace(atomIndex);
                    atoms[atomIndex].toSelected();
                    updateAtomMoves(atoms[atomIndex]);
                }
            }
        }
    }

    inline bool Partie::isInMap(Position &position)
    {
        return  position.x >= 0 && 
                position.y >= 0 && 
                position.x < level.width && 
                position.y < level.height;
    }

    void Partie::updateAtomMoves(Atom & atom){
            atom.movePosible.clear();
            for(auto direction : DIRECTION_VECTOR){
                Position position = (atom.data.position + direction.second);  
                if(level.map[position.y][position.x]){
                    bool isempty = true;
                    for(auto atom : atoms){
                        if(atom.data.position == position){
                            isempty = false;break;
                        }
                    }
                    if(isempty) atom.movePosible.emplace(direction.first);
                }              
            }
    }

    Partie::~Partie(){
        
    }

}
