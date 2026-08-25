#strict
#appendto _HH1
#appendto _HH2
#appendto _HH3
#appendto _HH4
#appendto _HH5

ActivateEntrance:
  if(gLocked==0)  return(SetEntrance(0),Sound("Locked"));
  if(!ActIdle())  return(1);
  SetAction("OpenDoor");
  Sound("DoorOpen");
  return(1);