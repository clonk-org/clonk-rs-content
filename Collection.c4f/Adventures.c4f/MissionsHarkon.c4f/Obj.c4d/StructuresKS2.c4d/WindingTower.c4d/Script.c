/*-- Förderturm --*/

#strict 2

#include DOOR
#include B_89

local Caller, Coal, PosX, PosY;
local Material2Mine, Object2Mine;
local mined;

protected func Initialize(){
  return(1);
  }

/* Kontext */

private func IsComplete()
{
  return (GetCon() >= 100); 
}