/*-- Bahre --*/

#strict

public HookOffset: return(-14);

protected Initialize:
   SetAction("Emty");
return(1);

ActivateEntrance:
   if (Contained(Par(0))) return(Aussteigen()); //Aussteigen
   if (Or(Not(CrewMember(Par(0))),Contents())) return(BlockEntrance(Par())); //Nur Crewmitglieder Ein-/Auslass gewähren; MAX 1 Clonk
   if (Not(SEqual(GetAction(),"Emty"))) return(BlockEntrance()); 
   SetOwner(GetOwner(Par(0)));
   SetDir(GetDir(Par(0)));
   SetAction("Trans");
   Enter(this(),Par(0));   
return(1);

private Aussteigen:
  Exit(Contents());
return(SetAction("Emty"));

private ClearCommand: //Par(0) Objekt
  SetVar(0,Par(0));
  if (Not(Var(0))) SetVar(0,this());
  SetCommand(Var(0),"None");
  return();

private BlockEntrance: //Par(0) Objekt
  ClearCommand(Par(0));
  if (InLiquid(Par(0))) return(SetComDir(COMD_Up(),Par(0)));  
  if (CrewMember(Par(0))) Jump(Par(0));
  return();



