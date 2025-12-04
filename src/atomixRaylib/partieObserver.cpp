#include "../atomixRaylib.hpp"

namespace Atomix
{

    void AtomixRaylib::onWin()
    {
        TraceLog(LOG_INFO, "WIN!");
    }
    void AtomixRaylib::onAtomSelected(Atom &selected)
    {
        Vector2 target = {(float)selected.data.position.x * CASE_SIZE, (float)selected.data.position.y * CASE_SIZE};
        camera.target = target;
        TraceLog(LOG_INFO, TextFormat("onAtomSelected %d", selected.id));
    }
    void AtomixRaylib::onAtomUnselected(Atom &unselected)
    {

        TraceLog(LOG_INFO, TextFormat("onAtomUnselected %d", unselected.id));
    }
    void AtomixRaylib::onAtomMove(Atom &atom, Position &origin, Position &dest)
    {
        Vector2 target = {(float)dest.x * CASE_SIZE, (float)dest.y * CASE_SIZE};
        camera.target = target;
        TraceLog(LOG_INFO, TextFormat("onAtomMove %d", atom.id));
    }
}