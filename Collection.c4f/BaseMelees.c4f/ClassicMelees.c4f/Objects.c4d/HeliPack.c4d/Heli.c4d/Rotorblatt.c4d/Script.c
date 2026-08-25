#strict

Hit:
  Sound("WoodHit*");
  return(1);

Decay:
  SetLocal(0,Sum(Local(0),+1));
  if (GreaterThan(Local(0),60)) RemoveObject();
  return(1);