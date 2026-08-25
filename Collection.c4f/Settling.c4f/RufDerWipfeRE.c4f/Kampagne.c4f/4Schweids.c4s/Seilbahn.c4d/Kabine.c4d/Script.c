/*-- Kabine --*/

#strict

local halter;

Travel:
  halter=CreateObject(SBHO);
  ObjectCall(halter,"Travel",Par(0),Par(1));
  return(SetAction("Ready",halter));

ContainedRight:
  [Nach oben fahren]
  ObjectCall(halter,"DriveUp");
  return(1);

ContainedLeft:
  [Nach unten fahren]
  ObjectCall(halter,"DriveDown");
  return(1);

Completion:
  SetEntrance(1);
  return(SetAction("Ready"));