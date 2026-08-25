/*-- Super-Flint --*/

#strict

#include FLNT


func Timer()
{
CreateParticle("PSpark",0,0,0,0,70,RGB(10,10,250));
if(FindObject(0,-8,-8,16,16,OCF_Alive()))Hit();
}
/* Aufschlag */

public func ExplodeSize() { return(30); }

/* Forschung */

public func GetResearchBase() { return(FLNT); }
