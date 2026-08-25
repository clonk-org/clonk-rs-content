#strict

Initialize:
  SetAction("Check");
  return(1);

Timer:
  if(SetVar(0,FindObject(KNIG,-1000,-1000,2000,2000)))  return(Erstellen(Var(0)));
  return(1);

Erstellen:
  if(!Random(3))  CreateObject(_LAV,-100+Random(201),-20+Random(41),-1);
  if(!Random(3))  CreateObject(_LAV,-100+Random(201),-20+Random(41),-1);
  return(1);