#strict

Initialize:
  SetAction("Check");
  return(1);

Timer:
  if(SetVar(0,FindObject(KNIG,-30,-1000,2000,2000)))  return(Erstellen(Var(0)));
  return(1);

Erstellen:
  SetPosition(0,0,this());

  Message("<%s>|Was ist das schon wieder| für ein Teufelswerkzeug?!",Par(0),GetName(Par(0)));
  Sound("Welt3_01",1);

//  Message("<%s>|Das Licht siegt über die Dunkelheit,| und so wird es dieses mal auch wieder sein!",Par(0),GetName(Par(0)));
//  Sound("Welt3_02",1);
  CreateObject(W_04,1510,190,-1);
  RemoveObject();
  return(1);