#strict

Initialize:
  SetAction("Fly");
  SetCategory(C4D_Background());
  return(1);

Turn:
  SetLocal(1,Par(0));
  SetR(Par(0));
  return(1);

Hit:
  RemoveObject();
  return(0);

Timer:
  if(!Random(5))  if(GBackSolid(0,0))  return(Hit());
  SetXDir(Cos(Local(1),70));
  SetYDir(Sin(Local(1),70));
  return(1);