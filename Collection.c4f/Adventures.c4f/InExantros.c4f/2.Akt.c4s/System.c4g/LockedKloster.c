#strict
#appendto _KL1

ActivateEntrance:
  if(gLocked2==0)  return(SetEntrance(0),Sound("Locked"));
  if(!ActIdle())  return(1);
  SetAction("OpenDoor");
  Sound("DoorOpen");
  return(1);

ContainedUp:
  SetVar(0,Par(0));
  Exit(Par(0));
  Enter(Local(2),Var(0));
  return(1);

