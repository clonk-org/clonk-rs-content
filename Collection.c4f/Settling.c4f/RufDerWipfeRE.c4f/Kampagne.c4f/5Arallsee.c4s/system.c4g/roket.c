#strict

#appendto _BAW

ResearchAllowed:
  if(!FindObject(RAKT,0,0,0,0,0,"IsDiscovered")) return(0);
  return(_inherited());