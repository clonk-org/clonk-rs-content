/*-- Toter Zombiewipf --*/

#strict

UndeadCheck:
  if(!Random(10))  return(Undead());
  return(1);

Undead:
  CreateObject(_WIF);
  RemoveObject();
  return(1);
