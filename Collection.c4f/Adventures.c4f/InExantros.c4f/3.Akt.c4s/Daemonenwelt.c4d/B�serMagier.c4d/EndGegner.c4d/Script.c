#strict

local itime,pboss,plicht,pleben,plage;

Initialize:
  var iplayer=GetPlayerCount()*2;
  itime=(GetPlayerCount()+1)/2;
  SetAction("Check");
  return(1);

Timer:
  if(SetVar(0,FindObject(KNIG,-30,-80,60,160)))  return(Erstellen(Var(0)));
  return(1);

Erstellen:
  SetPosition(0,0,this());
//Licht
  plicht=CreateObject(LI__,3100,250,-1);
  ObjectCall(plicht,"Licht",350);
//Der Endgegner
  pboss=CreateObject(_BB_,2900,330,-1);
  ObjectCall(pboss,"Gegner1",0,5);
  ObjectCall(pboss,"Lade",plicht,this());
//Weiter machen
  SetAction("Idle");
  Log("Letzter Endgegner: Der dunkle Wanderer");
  return(1);

Tot:
  plage=2;
  return(1);

Next:
//Ein Trick vom Endgegner
  pleben=Par(0);
  SetVar(0,Random(3));
  if(Var(0)==0)  ObjectCall(CreateObject(_GEG,0,0,-1),"Los",Par(1),Par(2)); //Gegner
  if(Var(0)==1)  ObjectCall(CreateObject(_GEG,0,0,-1),"Los",Par(1),Par(2)); //Gegner
  if(Var(0)==2)  ObjectCall(CreateObject(_GE2,0,0,-1),"Los",Par(1),Par(2)); //Feuerlöcher
  return(1);

Next2:
  if(plage==2)  return(GameCall("Abspann"),RemoveObject());
//Der Endgegner kämpft selbst mal
  var i =Random(3);
  if(i==0)  pboss=CreateObject(_BB_,2900,330,-1);
  if(i==1)  pboss=CreateObject(_BB_,3100,320,-1);
  if(i==2)  pboss=CreateObject(_BB_,3300,330,-1);
  ObjectCall(pboss,"Gegner1",0,5);
  ObjectCall(pboss,"Lade",plicht,this());
  ObjectSetAction(pboss,"Walk");
//Leben aktuallisieren
  DoEnergy(-GetEnergy(pboss)+pleben,pboss);
  return(1);

Start:
  if(plage==0)  {
  Message("<c ff0000> <Dunkle Gestalt>| Wer stört mich da?||Ihr fordert euer| Schicksal heraus!|Stirbt!</c>",pboss);
  Sound("Boss_01",1); 
  SoundLevel("EndKampf",99);
  CreateObject(G__E,0,0,-1);
		}
  plage=1;
  return(1);

Gegner:
  if(!pboss)  return(Start2());
  return(1);

Start2:
//Der Endgegner
  pboss=CreateObject(_BB_,2900,330,-1);
  ObjectCall(pboss,"Gegner1",0,5);
  ObjectCall(pboss,"Lade",plicht,this());
//Weiter
  Message("<c ff0000> <Dunkle Gestalt>| Ich habe bereits einen Teil der| Macht des Kritalls und ihr| werdet mir nicht hindern| alle Macht zu nehmen.</c>",pboss);
  SetAction("New2");
  return(1);

Gegner:
  if(!pboss)  return(Start3());
  return(1);

Start3:
//Der Endgegner
  pboss=CreateObject(_BM_,2900,330,-1);
  ObjectCall(pboss,"Gegner1",0,5);
  ObjectCall(pboss,"Lade",plicht,this());
//Weiter
  Message("<c ff0000> <Dunkle Gestalt>| Und wenn ich mich verwandle, dann habt ihr wohl nicht mehr zu lachen. HäHäHä</c>",pboss);
  SetAction("New2");
  return(1);

Ende:
  Music("Braveheart");
  GameCall("Win");
  Log("Ihr habt den Tümpelwächter bezwungen!");
  SetPosition(1920,1190,this());
  SetLocal(1,1,FindObject(FT__,-50,-80,100,160));
//  CreateObject(H_09,0,0,-1);
  RemoveObject();
  return(1);