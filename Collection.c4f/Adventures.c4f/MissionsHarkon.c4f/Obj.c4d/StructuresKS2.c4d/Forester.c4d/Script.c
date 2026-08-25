/*-- Förster --*/

#strict 2

#include DOOR

local Caller;

protected func Initialize(){
  CreateObject(BAS5,-18,34);
  SetPlrKnowledge(pCaller,KWTR); // Förderturm
  return(1);
  }

/* Kontext */

private func IsComplete()
{
  return (GetCon() >= 100); 
}