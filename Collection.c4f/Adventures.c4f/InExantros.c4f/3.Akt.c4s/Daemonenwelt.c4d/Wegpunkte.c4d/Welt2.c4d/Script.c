#strict

Initialize:
  SetAction("Check");
  return(1);

Timer:
  if(SetVar(0,FindObject(KNIG,-30,-1000,2000,2000)))  return(Erstellen(Var(0)));
  return(1);

Erstellen:
  SetPosition(0,0,this());

  Message("<%s>|Kommt nur, ihr dämonischen Bastarde!",Par(0),GetName(Par(0)));
  Sound("Welt2_01",1);
  CreateObject(_PF2,0,0,-1);
  RemoveObject();
  return(1);