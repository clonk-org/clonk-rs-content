/*-- Die Brücke --*/

#strict

// Global(Spielernummer) = Spielerposition
static const Position_Left = 1,
             Position_Right = 2;

static iInitializedPlayers;
static iArmorCount;

protected func Initialize()
{
  // Tolle Musik!
  Music("Territory.ogg");
  SetPlayList("Camelot.mid;Territory.ogg;Morning Dawn.mid;Rush.ogg");
  // Spielziel erstmal löschen (wird später wieder erzeugt)
  if(ObjectCount(MEL2)) RemoveObject(FindObject(MEL2));
  // Bäume und Pilze hinzufügen
  for(var i=0; i<20; i++)
  {
    PlaceVegetation(TRE1, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000-100*Random(Random(1000)));
    PlaceVegetation(TRE2, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000-100*Random(Random(1000)));
    PlaceVegetation(TRE3, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000-100*Random(Random(1000)));
    PlaceVegetation(TRE4, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000-100*Random(Random(1000)));
    if(!Random(15)) PlaceVegetation(MUSH, 0, 0, LandscapeWidth(), LandscapeHeight(), 100000-100*Random(Random(1000)));
  }
  // Hängebrücke platzieren
  CreateObject(_BRB, 1, 1, -1)->SetPosition(600,310);
  // Holztimer starten
  // StartWoodDisplay();
  // Ausrüstung des linken Teams erzeugen
  CreateVehicle(XBOW, 34, 314, -1, DIR_Right);
  var pLorry = CreateVehicle(LORY, 94, 328, -1, DIR_Right);
  CreateVehicle(CATA, 146, 315, -1, DIR_Right);
  //CreateVehicle(MS6C, 236, 314, -1, DIR_Right);
  CreateVehicle(MS6C, 464, 314, -1, DIR_Right);
  CreateObject(WT6C)->SetPosition(400, 297);
  pLorry->CreateContents(WOOD,, 5);
  pLorry->CreateContents(CNKT,, 4);
  pLorry->CreateContents(ARWP,, 10);
  pLorry->CreateContents(_GRT,, 10);
  pLorry->CreateContents(BOW1,, 2);
  pLorry->CreateContents(AXE1,, 3);
  pLorry->CreateContents(ARMR);
  // Ausrüstung des rechten Teams erzeugen
  CreateVehicle(XBOW, 1090, 314, -1, DIR_Left);
  var pLorry = CreateVehicle(LORY, 1033, 329, -1, DIR_Left);
  CreateVehicle(CATA, 974, 336, -1, DIR_Left);
  CreateVehicle(MS6C, 736, 313, -1, DIR_Left);
  //CreateVehicle(MS6C, 876, 324, -1, DIR_Left);
  CreateObject(WT6C)->SetPosition(811, 299);
  pLorry->CreateContents(WOOD,, 5);
  pLorry->CreateContents(CNKT,, 4);
  pLorry->CreateContents(ARWP,, 10);
  pLorry->CreateContents(_GRT,, 10);
  pLorry->CreateContents(BOW1,, 2);
  pLorry->CreateContents(AXE1,, 3);
  pLorry->CreateContents(ARMR);
}

protected func InitializePlayer(int iPlr, int iX, int iY, int irgendwas, int iTeam)
{
  // Scriptcounter starten
  ScriptGo(true);
  // Anzahl der beigetretenen Spieler erhöhen
  iInitializedPlayers++;
  // Team setzen
  Global(iPlr) = iTeam;
}

protected func RelaunchPlayer(int iPlr)
{
  iInitializedPlayers--;
  Global(iPlr) = 0;
}

protected func Script1()
{
  // Haben schon alle Spieler ausgewählt?
  if(iInitializedPlayers < GetPlayerCount()) return(goto(1));
  // Ziel: Melee Teamwork
  CreateObject(MEL2, 10,10,-1);

  // Anzahl der Rüstungen:
  var iArmorsPerTeam = Random(3); // 0-2 Rüstungen pro Team

  // Clonks der Spieler des linken Teams erzeugen und aufteilen
  var pClonk, iPlr = -1; iArmorCount=iArmorsPerTeam;
  // Ritter erzeugen
  CreateKnight( 90, 331, iPlr=GetNextPlayer(Position_Left,iPlr));
  CreateKnight(291, 338, iPlr=GetNextPlayer(Position_Left,iPlr));
  CreateKnight(184, 330, iPlr=GetNextPlayer(Position_Left,iPlr));
  CreateKnight(333, 329, iPlr=GetNextPlayer(Position_Left,iPlr));
  // Falls mehr Spieler im Team sind, wollen die natürlich auch Clonks
  if(GetTeamPlayerCount(Position_Left) > 4)
    for(var i=4; i<GetTeamPlayerCount(Position_Left); i++)
    {
      var m = i%4;
           if(m == 0)
             CreateKnight( 31, 318, iPlr=GetNextPlayer(Position_Left,iPlr));
      else if(m == 1)
             CreateKnight(140, 318, iPlr=GetNextPlayer(Position_Left,iPlr));
      else if(m == 2)
             CreateKnight(235, 318, iPlr=GetNextPlayer(Position_Left,iPlr));
      else if(m == 3)
             CreateKnight(391, 318, iPlr=GetNextPlayer(Position_Left,iPlr));
    }

  // Clonks der Spieler des rechten Teams erzeugen und aufteilen
  var pClonk, iPlr = -1; iArmorCount=iArmorsPerTeam;
  // Ritter erzeugen
  CreateKnight( 865, 328, iPlr=GetNextPlayer(Position_Right,iPlr));
  CreateKnight(1066, 316, iPlr=GetNextPlayer(Position_Right,iPlr));
  CreateKnight( 959, 339, iPlr=GetNextPlayer(Position_Right,iPlr));
  CreateKnight(1108, 319, iPlr=GetNextPlayer(Position_Right,iPlr));
  // Falls mehr Spieler im Team sind, wollen die natürlich auch Clonks
  if(GetTeamPlayerCount(Position_Left) > 4)
    for(var i=4; i<GetTeamPlayerCount(Position_Left); i++)
    {
      var m = i%4;
           if(m == 0)
             CreateKnight( 806, 319, iPlr=GetNextPlayer(Position_Right,iPlr));
      else if(m == 1)
             CreateKnight( 915, 317, iPlr=GetNextPlayer(Position_Right,iPlr));
      else if(m == 2)
             CreateKnight(1010, 329, iPlr=GetNextPlayer(Position_Right,iPlr));
      else if(m == 3)
             CreateKnight(1166, 328, iPlr=GetNextPlayer(Position_Right,iPlr));
    }

  // Alle Teams verfeinden
  for(var i=0; i<GetPlayerCount(); i++)
    for(var j=0; j<GetPlayerCount(); j++)
      if(Global(GetPlayerByIndex(i)) != Global(GetPlayerByIndex(j)))
        SetHostility(GetPlayerByIndex(i),GetPlayerByIndex(j),1,1);
      else
        SetHostility(GetPlayerByIndex(i),GetPlayerByIndex(j),0,1);

  // Kameraobjekte löschen
  RemoveAll(_CAM);  
  // Jeder Spieler hat seinen ersten Clonk ausgewählt
  for(var i=0; i<GetPlayerCount(); i++)
  {
    var iPlr = GetPlayerByIndex(i);
    UnselectCrew(iPlr);
    SelectCrew(iPlr, GetCrew(iPlr), 1);
    SetPlrView(iPlr, GetCursor(iPlr));
  }
  //ScriptCounter stoppen
  ScriptGo(0);

  return(1);
}

public func GetNextPlayer(int iTeam, int iPlr)
{
  var plr = iPlr;
  while(true)
  {
    plr++;
    if(iTeam==Global(plr)) return(plr);
    if(plr>GetPlayerCount()) plr = -1;
    if(plr==iPlr) return(plr);
  }
}

public func GetTeamPlayerCount(int iTeam)
{
  var c;
  for(var i=0; i<GetPlayerCount(); i++)
    if(Global(GetPlayerByIndex(i)) == iTeam)
      c++;
  return c;
}

// Erzeugt einen zufällig ausgerüsteten Ritter
private func CreateKnight(int iX, int iY, int iOwner)
{
  var knight = CreatePlrObj(KNIG, iOwner, iX, iY, 1);
  // Schild
  if(Random(2))
    CreateObject(SHIE, 10,10, iOwner)->Activate(knight);
  // Rüstung
  if(iArmorCount-- > 0)
  {
    if(!Random(10))
      knight->SetArmored(1, BRMR);
    else
      knight->SetArmored(1, ARMR);
  }
  // Axt oder Schwert
  if(!Random(3))
    CreateContents(AXE1, knight);
  else
    CreateContents(SWOR, knight);
  // Speer
  if(!Random(4))
    CreateContents(SPER, knight);
  return(knight);
}

// Erzeugt ein Objekt der angegebenen ID und fügt es ggf. der Mannschaft eines Spielers hinzu.
private func CreatePlrObj(id, int iPlr, int iX, int iY, fIsCrew, int Rand)
{
  if(!Rand) Rand = 50;
  var pObj = CreateObject(id, iX+Random(Rand*2+1)-Rand,iY, iPlr);
  if(fIsCrew)
  {
    if(iPlr != -1) MakeCrewMember(pObj, iPlr);
    DoEnergy(GetPhysical("Energy", 0, pObj) / 1000, pObj);
  }
  return(pObj);
}

protected func CreateVehicle(id Id, int iX, int iY, int iOwner, int iDir)
{
  var pObj = CreateObject(Id, iX, iY, iOwner);
  pObj->SetDir(iDir);
  return(pObj);
}

// Beitrittsverbot schon nach 5 Frames
public func NJ6C_GetTimeSpan() { return(5); }

