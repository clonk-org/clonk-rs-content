/*-- Holzhütte I --*/

#strict 2
local Caller;

#include DOOR
#include B_89
static pCaller;

protected func Initialize(){
  return(1);
  } 
/* Kontext */

private func IsComplete()
{
  return (GetCon() >= 100); 
}