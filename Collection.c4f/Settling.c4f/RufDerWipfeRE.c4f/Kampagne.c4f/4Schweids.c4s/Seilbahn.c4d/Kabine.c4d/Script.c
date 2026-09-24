/*-- Kabine --*/

#strict

local halter;

Travel:
  halter=CreateObject(SBHO);
  ObjectCall(halter,"Travel",Par(0),Par(1));
  return(SetAction("Ready",halter));

ContainedRight:
  [$CapUp$]
  ObjectCall(halter,"DriveUp");
  return(1);

ContainedLeft:
  [$CapDown$]
  ObjectCall(halter,"DriveDown");
  return(1);

Completion:
  SetEntrance(1);
  return(SetAction("Ready"));