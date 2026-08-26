/*-- Fahrstuhlkorb --*/

#strict 2

#include CLNK
#appendto CLNK 

protected func Death() {
    var pung = FindObject2(Find_ID(938Z),Find_ActionTarget(this));
    if (pung) RemoveObject(pung);
    return( _inherited());
}