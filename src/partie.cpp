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
            if(atoms[selected.value()].data.position != position){
                atoms[selected.value()].toWait();
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

    void Partie::updateAtomSelectedMoves(){
        movesPossible.clear();
        if(selected.has_value()){
            for(auto direction : DIRECTION_VECTOR){
                Position position = (atoms[selected.value()].data.position + direction.second);  
                if(level.map[position.y][position.x]){
                    bool isempty = true;
                    for(auto atom : atoms){
                        if(atom.data.position == position){
                            isempty = false;break;
                        }
                    }
                    if(isempty) movesPossible.emplace(direction.first);
                }              
            }
        }
        
    }

    Partie::~Partie(){
        
    }

}
