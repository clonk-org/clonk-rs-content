/*-- Der Aufstand --*/

#strict

// Global(Spielernummer) = Spielerposition
static const Position_Castle = 1,
             Position_Village = 2;

static TeamInitialized;
static iInitializedPlayers;

public func Initialize()
{
  // Musik!
  Music("Rush.ogg");
  SetPlayList("Rush.ogg;Darkside.mid;The Chant.ogg");
  // Variablen initialisieren
  TeamInitialized  = [0,false,false];
  // Alle vorhandenen Ziele und Regeln löschen
  var obj;
  while(obj=FindObject(,,,,,,,,,obj))
    if((GetCategory(obj) & C4D_Goal) || (GetCategory(obj) & C4D_Rule))
      RemoveObject(obj);    
  // Es ist Nacht
  InitNight();
  // Sterne erzeugen
  for(var i=0; i<250; i++)
    CreateObject(STAR, Random(LandscapeWidth()), Random(LandscapeHeight()), -1);
  // Erdreichobjekte und Nester
  PlaceObjects(LOAM,30,"Earth");
  PlaceObjects(FLNT,30/5,"Earth");
  PlaceObjects(GOLD,30*3/5,"Earth");
  PlaceObjects(ROCK,30*4/5,"Earth");
  PlaceObjects(SKUL,2,"Earth");
  PlaceObjects(ZAPN,2,"Earth");
  PlaceObjects(MEGG,1,"Earth");
  PlaceObjects(FMEG,1,"Earth");      
  // Tiere
  for(var i=0; i<15; i++)
    PlaceAnimal(FISH);     // 15 Fische
  for(var i=0; i<3; i++)
    PlaceAnimal(SHRK);     // 3 Haie
  for(var i=0; i<5; i++)
    PlaceAnimal(BIRD);     // 5 Vögel
  // Bäume erzeugen
  for(var i=0; i<5; i++)
  {
    PlaceVegetation(TRE1, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
    PlaceVegetation(TRE2, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
    PlaceVegetation(TRE3, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
    PlaceVegetation(TRE4, 0, 0, LandscapeWidth(), LandscapeHeight(), -1);
  }
  // Bäume hinter Burgteile
  ResortObjects("ResortPlantsProc", C4D_StaticBack);
  // Gebäude sind nicht betretbar
  var pObj;
  while(pObj = FindObject(,,,,,,,,,pObj))
    if(GetCategory(pObj) & C4D_Structure)
      SetEntrance(0);
}

protected func InitializePlayer(int iPlr, int iX, int iY, int irgendwas, int iTeam)
{
  // Scriptcounter starten
  ScriptGo(true);
  // Anzahl der beigetretenen Spieler erhöhen
  iInitializedPlayers++;
  // Team setzen
  Global(iPlr) = iTeam;
  TeamInitialized[iTeam] = true;
}

protected func RelaunchPlayer(int iPlr)
{
  iInitializedPlayers--;
  Global(iPlr) = 0;
  // Team-Hud entfernen
  var obj;
  for(obj in FindObjects(Find_ID(_THD), Find_Owner(iPlr)))
    RemoveObject(obj);
}

protected func ResortPlantsProc(object obj1, object obj2)
{
  if(obj2->~IsTree()) return(-1);
}

protected func Script1()
{
  // Haben schon alle Spieler ausgewählt?
  if(iInitializedPlayers < GetPlayerCount()) return(goto(1));

  // Sind Spieler in beiden Teams?
  if(!TeamInitialized[Position_Castle])
  {
    Message("<c cccccc>$Empty1$");
    Log("$Empty1$");
    return(GameOver());
  }
  if(!TeamInitialized[Position_Village])
  {
    Message("<c cccccc>$Empty2$");
    Log("$Empty2$");
    return(GameOver());
  }

  // Ziel: Melee Teamwork
  CreateObject(MEL2, 10,10,-1);
  // Regeln
  CreateObject(SURR, 10,10,-1); // Aufgeben erlaubt
  CreateObject(SCAH, 10,10,-1); // Klettern & Hangeln
  CreateObject(KILC, 10,10,-1); // Verlust Captain
  CreateObject(BK7U, 10,10,-1); // Brennbare Könige
  CreateObject(NJ6C, 10,10,-1); // Spielerbeitrittsverbot
  CreateObject(_ETG, 10,10,-1); // Löscher
  // Umweltobjekte
  CreateObject(OWL1, 10,10,-1); // Eulen
  CreateObject(BRDS, 10,10,-1); // Vogelgesang
  CreateObject(CICA, 10,10,-1); // Zikaden
  CreateObject(EGLN, 10,10,-1); // Glitzernde Materialien
  CreateObject(CLFS, 10,10,-1); // Bunte Fische
  //CreateObject(CC6C, 10,10,-1); // Bunte Clonks
  // Zugbrücke öffnen
  FindObject(CPT3)->OpenDrawbridge();

  // Clonks der Spieler in der Burg erzeugen und aufteilen
  var pClonk, iPlr = GetNextPlayer(Position_Castle, GetPlayerByIndex(Random(GetPlayerCount())));
  // König erzeugen
  MakeCaptain(pClonk = CreatePlrObj(KING, iPlr, 520,390, 1));  // Der König ist Captain
  SetName("Thanatos", FindObject(KING));
  // HUD erzeugen
  var iHudY = 75+16;
  CreateObject(_THD, 45, iHudY)->BindTo(pClonk); iHudY+=5;
  // Waffen erzeugen
  CreateContents(SWOR, pClonk);
  if(Random(2)) CreateObject(SHIE, 10,10, iPlr)->Activate(pClonk);
  // Message ist über dem König
  Message("<c %x>$Msg1$", pClonk, GetColorDw(pClonk));
  // Banner erzeugen
  CreateObject(BANR, 399, 250+25, iPlr)->UpdateColor();
  CreateObject(BANR, 592, 250+25, iPlr)->UpdateColor();
  CreateObject(BANR, 712, 310+25, iPlr)->UpdateColor();
  // Burg gehört dem ersten Spieler aus der Burg
  //SetObjOwner(iPlr, 350,200, 800,420);
  // Zuviele Spieler im Team?
  if(GetTeamPlayerCount(Position_Castle) <= 6)
  {
    // Mindestens fünf Ritter, auch wenn weniger Spieler da sind
    for(var i=0; i<5; i++)
      CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateKnight(520,390, iPlr=GetNextPlayer(Position_Castle,iPlr)));
  }
  else
  {
    // Jeder Spieler bekommt einen Ritter
    for(var i=1; i<GetTeamPlayerCount(Position_Castle); i++)
      CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateKnight(520,390, iPlr=GetNextPlayer(Position_Castle,iPlr)));
  }

  // Clonks der Spieler im Dorf erzeugen und aufteilen
  iPlr = GetNextPlayer(Position_Village, GetPlayerByIndex(Random(GetPlayerCount())));
  // Ritter erzeugen
  MakeCaptain(pClonk = CreateKnight(1506,490, iPlr)); // Der Ritter ist Captain
  // Ritter im Dorf hat immer eine Rüstung
  if(!pClonk->HasArmor())
    if(!Random(7))
      pClonk->SetArmored(true, BRMR);
    else
      pClonk->SetArmored(true, ARMR);
  // HUD erzeugen
  var iHudY = 75+16;
  CreateObject(_THD, 45, iHudY)->BindTo(pClonk); iHudY+=5;
  // Message ist über dem Ritter
  Message("<c %x>$Msg2$", pClonk, GetColorDw(pClonk));
  // Ritter bekommt Bogen und Pfeile
  for(var i=Random(2)+2; i>0; i--)
    if(Random(5))
      CreateContents(ARWP, pClonk);
    else
      if(Random(5))
        CreateContents(XARP, pClonk);
      else
        CreateContents(FARP, pClonk);
  CreateContents(BOW1, pClonk);
  // Ein Banner für das Dorf
  CreateObject(BANR, 1478, 432+25, iPlr)->UpdateColor();
  // Dorf gehört dem ersten Spieler aus dem Dorf
  //SetObjOwner(iPlr, 1070,350, 1720,590);
  // Zuviele Spieler im Team?
  if(GetTeamPlayerCount(Position_Village) <= 6)
  {
    // Fünf Bauern erzeugen
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1250,545, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1564,485, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1420,510, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1295,545, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1650,423, iPlr=GetNextPlayer(Position_Village, iPlr)));
  }
  else
  {
    // Fünf Bauern erzeugen
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1250,545, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1564,485, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1420,510, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1295,545, iPlr=GetNextPlayer(Position_Village, iPlr)));
    CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1650,423, iPlr=GetNextPlayer(Position_Village, iPlr)));
    // Für die restlichen Spieler auch noch Bauern erzeugen
    for(var i=6; i<GetTeamPlayerCount(Position_Village); i++)
      CreateObject(_THD, 45, iHudY+=37)->BindTo(CreateClonk(1405+Random(30),510, iPlr=GetNextPlayer(Position_Village, iPlr)));
  }

  // Alle Spieler in der Burg mit allen Spielern aus dem Dorf und umgekehrt verfeinden
  for(var i=0; i<GetPlayerCount(); i++)
    for(var j=0; j<GetPlayerCount(); j++)
      if(Global(GetPlayerByIndex(i)) != Global(GetPlayerByIndex(j)))
        SetHostility(GetPlayerByIndex(i),GetPlayerByIndex(j),1,1);
      else
        SetHostility(GetPlayerByIndex(i),GetPlayerByIndex(j),0,1);

  // Siedlungspunkte aller Spieler auf 0 setzen
/*
  for(var i=0; i<GetPlayerCount(); i++)
    SetPlrValue(0, GetPlayerByIndex(i));
*/

  // Fader von ganz dunkel bis normal starten
  Fade(0, 128, 2, 1, 200);
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
  var knight = CreatePlrObj(KNIG, iOwner, iX, iY, true);
  // Schild
  if(Random(3))
    CreateObject(SHIE, 10,10, iOwner)->Activate(knight);
  // Rüstung
  if(!Random(5))
  {
    if(!Random(7))
      knight->SetArmored(true, BRMR);
    else
      knight->SetArmored(true, ARMR);
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

// Erzeugt einen Clonk, der mit einem Flint bewaffnet ist
private func CreateClonk(int iX, int iY, int iOwner)
{
  var clonk = CreatePlrObj(CLNK, iOwner, iX, iY, true, 1);
  CreateFlint(clonk);
  if(Random(2)) CreateFlint(clonk);
  return(clonk);
}

private func CreateFlint(object container)
{
  var flint;
  // Brandbombe oder Flint
  if(!Random(5))
    flint = CreateContents(FBMP, container);
  else
    // T-Flint oder normales Flint
    if(!Random(3))
      // T-Flint oder Super-T-Flint
      if(Random(2))
        flint = CreateContents(TFLN, container);
      else
        flint = CreateContents(STFN, container);    
    else
      // Feuerstein oder besseres
      if(Random(3))
        flint = CreateContents(FLNT, container);
      else
        // Superflint oder Teraflint
        if(Random(2))
          flint = CreateContents(SFLN, container);
        else
          if(!Random(4))
            flint = CreateContents(EFLN, container);
          else
            if(Random(3))
              flint = CreateContents(ZAPN, container);
            else
              flint = CreateContents(MEGG, container);
  return(flint);
}

// Erzeugt ein Objekt der angegebenen ID und fügt es ggf. der Mannschaft eines Spielers hinzu.
private func CreatePlrObj(id, int iPlr, int iX, int iY, bool fIsCrew, int Randomness)
{
  if(!Randomness) Randomness = 50;
  var pObj = CreateObject(id, iX+Random(Randomness*2+1)-Randomness,iY, iPlr);
  if(fIsCrew)
  {
    if(iPlr != -1) MakeCrewMember(pObj, iPlr);
    DoEnergy(GetPhysical("Energy", 0, pObj) / 1000, pObj);
  }
  return(pObj);
}

private func SetPlrObjects(iPlr, iX, iY)
{
  var pObj;
  while(pObj = FindObjectOwner(0, iPlr, 0,0,0,0, 0,0,0, pObj))
    SetPosition(iX+Random(51)-25, iY, pObj);
  return(1);
}

private func SetObjOwner(iPlr, iX, iY, iX2, iY2)
{
  var pObj;
  while(pObj = FindObject(0, iX, iY, iX2 - iX, iY2 - iY, 0,0,0,0, pObj))
    if(GetOwner(pObj) == -1)
      if((GetID(pObj) != FISH) &&
         (GetID(pObj) != BIRD) &&
         (GetID(pObj) != SHRK))
        SetOwner(iPlr, pObj);
  return(1);
}

// Beitrittsverbot schon nach 5 Frames
public func NJ6C_GetTimeSpan() { return(5); }

