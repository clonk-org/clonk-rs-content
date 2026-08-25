/*-- Castle Defense --*/

#strict

func Initialize()
{
  var Torch;
  while(Torch=FindObject(TORC, 0, 0, 0, 0, 0, 0, 0, 0, Torch))
    if(Not(SEqual(GetAction(Torch),"Burning")))
      ObjectCall(Torch, "Activate");
  
  // Das Spark-Objekt für Zufallsobjekte vom Himmel initialisieren
  RemoveAll(_SPK);
  CreateObject(_SPK, LandscapeWidth()/2 ,-5, -1)->SetAction("Wait");
  CreateObject(_SPK,LandscapeWidth()/2+28,0,-1);
  
  // In der Objects.txt steckt ein Zaehler-Objekt, das Mist baut. In der
  // Objects.txt zu pfuschen ist aber gefaehrlich, also loeschen wir das
  // lieber hier.
  RemoveAll(_CTD);
  RemoveAll(_ZAE);
  RemoveAll(CNMT);
  RemoveObject(FindObject(ENRG));
  ScriptGo(1);
  SetSkyParallax(1, 20, 20, 0, 0, SkyPar_Keep(), SkyPar_Keep());
  DigFreeRect(890, 250, 410, 10);
  RemoveObject(FindObject(TACC));
  RemoveAll(_SPC);
  // Holigon einlagern leicht gemacht
  RemoveAll(BHLG);
  for(var altar in FindObjects(Find_ID(ALTR)))
  {
    CreateContents(BHLG,altar,3); 
    altar->RefreshAction();
  }
  // Zeug verbuddeln
  Remove(FLNT);
  Remove(ROCK);
  Remove(LOAM);
  Remove(GOLD);
  Create(FLNT,RandomX(4,8));
  Create(ROCK,RandomX(5,9));
  Create(LOAM,RandomX(3,5));
  Create(GOLD,RandomX(4,9));
  if(!ObjectCount(MELE)) CreateObject(MELE, 0, 0, NO_OWNER);
}

protected func Remove(id ID)
{
  for(var i in FindObjects(Find_ID(ID),Find_OCF(OCF_NotContained)))
    RemoveObject(i);
}

protected func Create(id ID, int iAmmount)
{
  for(var i = iAmmount; i>0; i--)
    PlaceInMaterial(ID,Material("Earth"));
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
 // Den Spieler ausrüsten und platzieren
 SetOwner(player, CreateContents(FLAG, GetCrew(player,1)));
 SetOwner(player, CreateObject(_CTD, 0, 0, player));
 CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player, iTeam);
 if(iTeam == 1)
 {
   SetPlrKnowledge(player, XBOW); // Bauplan: Armbrust
   SetPlrKnowledge(player, BALN); // Bauplan: Balon
   SetPlrKnowledge(player, CATA); // Bauplan: Katapult
   SetWealth(player, 220);        // Startguthaben der Angreifer
   for(var crew = 0; crew < GetCrewCount(player); crew++)
     SetPosition(RandomX(70,110),300,GetCrew(player,crew));
 }
 else
 {
   for(var crew = 0; crew < GetCrewCount(player); crew++)
     SetPosition(RandomX(1100,1150),250,GetCrew(player,crew));
    //burgfahrstuhl finden
    var pElevator = FindObject2(Find_ID(CPEL));
    //eigentümer des burgfahrstuhls setzen, der letzte wird endgültiger
    SetOwner(GetOwner(GetCrew(player)),pElevator);
    pElevator = LocalN("pCase",pElevator);
    SetOwner(GetOwner(GetCrew(player)),pElevator);
 }
 return(1);
}


 protected func RemovePlayer(int player)
{
 // Spielervariable zurücksetzen, damit Laufzeitbeitritt mit Aufgeben klappt
 Global(player) = 0;
 return(1);
}

protected func Eliminate()
{
  for(var player = 0; player < GetPlayerCount(); player++)
    if(GetPlayerTeam(player)==2)
      SurrenderPlayer(player);
}

protected func TimeUp()
{
  for(var i = GetPlayerCount();i>0;i--)
    if(GetPlayerTeam(i)==2) break;       // Besitzer
  var pmage=CreateObject(MAGE,0,0,i);    // Magus
  var pdrag=CreateObject(DRGN,800,100,i);// Drache
  MakeCrewMember(pmage,i);
  SetCursor(i,pmage);
  while(DoMagicEnergy(+1,pmage)) continue;
  DoEnergy(1000,pmage);
  pdrag->SetAction("Fly");
  pmage->SetAction("Ride",pdrag);
  CastParticles("PSpark",200,100,800,50,50,70,0,0);
  SetDir(DIR_Left,pdrag);
}
