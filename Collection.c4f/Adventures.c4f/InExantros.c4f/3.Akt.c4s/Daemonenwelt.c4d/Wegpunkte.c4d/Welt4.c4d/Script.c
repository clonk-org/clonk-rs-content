#strict

Initialize:
  SetAction("Check");
  return(1);

Timer:
  if(SetVar(0,FindObject(KNIG,-30,-1000,2000,2000)))  return(Erstellen(Var(0)));
  return(1);

Erstellen:
  SetPosition(0,0,this());

  Message("<%s>|Dunkler Wanderer euer| Ende wird bald kommen.|Diese Pforten sind keine| Herausforderung für mich!",Par(0),GetName(Par(0)));
  Sound("Welt4_01",1);
  GameCall("NextPforte");
  CreateObject(_PF3,0,0,-1);
  RemoveObject();
  return(1);