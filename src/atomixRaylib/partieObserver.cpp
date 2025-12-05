#include "../atomixRaylib.hpp"

namespace Atomix
{

    void AtomixRaylib::onWin()
    {
        TraceLog(LOG_INFO, "WIN!");
    }
    void AtomixRaylib::onAtomSelected(Atom &selected)
    {
        TraceLog(LOG_INFO, TextFormat("onAtomSelected %d", selected.id));
    }
    void AtomixRaylib::onAtomUnselected(Atom &unselected)
    {
        TraceLog(LOG_INFO, TextFormat("onAtomUnselected %d", unselected.id));
    }
    void AtomixRaylib::onAtomMove(Atom &atom, Position &origin, Position &dest)
    {
        TraceLog(LOG_INFO, TextFormat("onAtomMove %d", atom.id));
    }
}