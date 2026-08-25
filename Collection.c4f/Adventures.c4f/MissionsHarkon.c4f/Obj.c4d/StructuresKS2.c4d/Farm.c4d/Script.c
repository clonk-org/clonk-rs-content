/*-- Weizenfarm --*/

#strict 2

#include DOOR
#include B_89

protected func Initialize(){
  SetAction("Idle");
  return(1);
  }

/* Kontext */

private func IsComplete()
{
  return (GetCon() >= 100); 
}
  
protected func doSmoke(){
  Smoke(10,-20,25);
  Sound("Chuff");
  return(1);
  }
