/*-- Seilbahnstation --*/

#strict
#include B070

BasementID: return(B080);

TurnRight:
  SetSolidMask(0,35,75,25,5,35);
  SetAction("Right");
  return(1);

TurnLeft:
  SetSolidMask(85,35,75,25,5,35);
  SetAction("Left");
  return(1);