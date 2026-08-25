/*-- Ritter --*/

#strict

#appendto CLNK

protected func Collection2 (pObj) {
SetOwner(GetOwner(this()),pObj);
_inherited (...);
}