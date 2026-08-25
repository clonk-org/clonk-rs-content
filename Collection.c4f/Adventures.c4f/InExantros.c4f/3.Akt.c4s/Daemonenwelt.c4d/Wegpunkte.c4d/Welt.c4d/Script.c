#strict

Initialize:
  SetAction("Check");
  return(1);

Timer:
  if(SetVar(0,FindObject(KNIG,-30,-1000,2000,2000)))  return(Erstellen(Var(0)));
  return(1);

Erstellen:
  SetPosition(0,0,this());

  Log("Ihr habt %d ingesamt Leben (Relaunches) ",gleben);

  Message("<%s>|Üble Kraturen und Dämonen, ihr werdet all euer Ende finden.",Par(0),GetName(Par(0)));
  Sound("Welt_01",1);
  GameCall("Teil1");
  CreateObject(_PF1,0,0,-1);
  RemoveObject();
  return(1);