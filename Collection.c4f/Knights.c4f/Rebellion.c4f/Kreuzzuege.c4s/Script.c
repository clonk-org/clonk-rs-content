/*-- Kreuzzüge --*/

#strict

// Farbe 1: 1997010 (Helleres Blau)
// Farbe 2: 13793310 (Rostrot)

static iDifficulty;
static iMaxRelaunchCount;

static GameObjectsCreated;

static aHut, aRelaunch;

protected func Initialize()
{
  iDifficulty = -1;
  // Musik aus KnightShift!
  Music("@Theft.ogg");
  SetPlayList("Rush.ogg;Darkside.mid;Territory.ogg;Camelot.mid");
  // ObjectNumbers der bewohnbaren Hütten (max. 6 Spieler)
  // Relaunch ja/nein?
  aRelaunch = CreateArray(6);
  // Spieler 0 wohnt in aHut[0], usw.
  aHut = [Object(1405), Object(1490), Object(1486),
          Object(1492), Object(1477), Object(1494)];
  // Goldstatue markieren (damit sie nicht vom Ziel gefunden wird!)
  FindObject(GIDL)->LocalN("film_object") = true;
  // Fahrstühle "reparieren"
  var pObj;
  if(pObj=FindObject(CPEC)) LocalN("RangeTop", pObj) = GetY(pObj);
  if(pObj=FindObject(ELEC)) LocalN("RangeTop", pObj) = GetY(pObj);
  // Fahrstuhlschacht graben
  FindObject(CPEL)->CreateShaft(100);
}

protected func InitializePlayer(int iPlr)
{
  // Objekte erzeugen
  CreateGameObjects();
  // Fog of War
  SetFoW(true, iPlr);
  // Den Spieler in seine Hütte setzen
  if(Inside(iPlr, 0, 5))
  {
    aHut[iPlr]->SetOwner(iPlr);
    GetCrew(iPlr)->Enter(aHut[iPlr]);
    CreateContents(FLAG, aHut[iPlr]);
    // Energieleiste
    var pEnergy = CreateObject(_EBR,,,iPlr);
    pEnergy->AttachTo(GetCrew(iPlr));
    pEnergy->SetAllied(true);
    // Wenn Schwierigkeitsgrad bereits gewählt
    if(iDifficulty > -1)
      // Ausrüstung erzeugen
      CreatePlayerItems(iPlr);
  }
  else 
    // Der Spieler hat keine Hütte?
    // Sollte zwar nicht vorkommen, aber zur Sicherheit in irgendeine Hütte stecken...
    GetCrew(iPlr)->Enter(aHut[Random(6)]);
}

protected func RelaunchPlayer(int iPlr)
{
  if(!Inside(iPlr, 0, 5)) return(false);
  // Relaunch-Zahl erhöhen
  aRelaunch[iPlr]++;
  // Alle Relaunchs verbraucht?
  if(aRelaunch[iPlr] > iMaxRelaunchCount)
  {
    // Relaunchs zurücksetzen
    aRelaunch[iPlr] = 0;
    // Spieler eliminieren
    EliminatePlayer(iPlr);
    return(true);
  }
  // Nachricht ausgeben
  Log("$Relaunch$", GetPlayerName(iPlr));
  // Ritter erzeugen
  var clonk = CreateObject(KNIG, 10,10, iPlr);
  MakeCrewMember(clonk, iPlr);
  SelectCrew(iPlr, clonk, true);
  // Energieleiste
  var pEnergy = CreateObject(_EBR,,,iPlr);
  pEnergy->AttachTo(clonk);
  pEnergy->SetAllied(true);
  // Den Spieler in seine Hütte setzen
  GetCrew(iPlr)->Enter(aHut[iPlr]);
  // Neue Ausrüstung erzeugen
  CreatePlayerItems(iPlr);
  return(true);
}

private func CreatePlayerItems(int iPlr)
{
  if(Random(2)) CreateContents(SWOR, aHut[iPlr]);
  else CreateContents(AXE1, aHut[iPlr]);
  if(!Random(3)) CreateContents(SHIE, aHut[iPlr]);
  if(!Random(3+iDifficulty)) CreateContents(SPER, aHut[iPlr]);
  if(!Random(4+iDifficulty))
  {
    CreateContents(BOW1, aHut[iPlr]);
    CreateContents(ARWP, aHut[iPlr]);
    CreateContents(FARP, aHut[iPlr]);
    if(!Random(2)) CreateContents(FARP, aHut[iPlr]);
    if(!Random(2)) CreateContents(XARP, aHut[iPlr]);
  }
  if(!Random(4+iDifficulty*2)) CreateContents(BRED, aHut[iPlr]);
  if(!Random(10+iDifficulty)) CreateContents(PSTO, aHut[iPlr]);
  if(!Random(20+iDifficulty*5)) CreateContents(ARMR, aHut[iPlr]);
  else if(!Random(80+iDifficulty*20)) CreateContents(BRMR, aHut[iPlr]);
}

protected func Script1()
{
  // Film starten
  CreateObject(_F00,10,10,-1)->StartFilm();
  // ScriptCounter anhalten (wird nach Ende von Film wieder gestartet)
  ScriptGo(false);
}

protected func Script2()
{
  var iPlr = GetPlayerByIndex(0);
  var clonk = GetCrew(iPlr);
  // Alle anderen Clonks deaktivieren
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
  {
    obj->SetComDir(COMD_None);
    if(obj != clonk) obj->SetCrewEnabled(false);
  }
  // Kein Spielerbeitritt während Auswahl!
  SetMaxPlayer(0);
  // Menüobjekt erzeugen
  CreateObject(_DSL)->ShowMenu(clonk);
  // Nachricht
  Message("@<c %x>%s</c><c cccccc> $HostChoosesDifficulty$", 0, GetPlrColorDw(iPlr), GetPlayerName(iPlr));
}

protected func Script3()
{
  // Gamma-Bugfix (besser nochmal)
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 0);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 2);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 4);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 5);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 6);
  SetGamma(RGB(0,0,0), RGB(128,128,128), RGB(255,255,255), 7);
  // Auf Menüauswahl warten
  if(FindObject(_DSL)) return(goto(3));
  // Nachricht
  if(iDifficulty == 0)
    Message("<c cccccc>$Easy$");
  else if(iDifficulty == 1)
    Message("<c cccccc>$Normal$");
  else if(iDifficulty == 2)
    Message("<c cccccc>$Hard$");
  // Clonks wieder aktivieren
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember)))
  {
    obj->SetCrewEnabled(true);
    SelectCrew(GetOwner(obj), obj, true);
  }
  // Spielerbeitritt wieder erlauben
  SetMaxPlayer(6);
  // Anzahl der Relaunchs
  if(iDifficulty < 2) iMaxRelaunchCount = 1;
  // Items erzeugen
  for(var i=0; i<GetPlayerCount(); i++)
    CreatePlayerItems(GetPlayerByIndex(i));
  // KI-Clonks erzeugen:
  // Feldlager rechts
  if(iDifficulty > 0) CreateBowmanKI(2335, 385, 300);
  CreateBowmanKI(2083, 336, 300);
  CreateKnightKI(2281, 439, 300);
  CreateKnightKI(2154, 438, 300);
  CreateKnightKI(2030, 390, 300);
  // Burg unten rechts
  CreateBowmanKI(2916, 706, 400);
  if(iDifficulty > 1) CreateBowmanKI(2997, 646, 400);
  CreateKnightKI(2421, 740, 400);
  if(iDifficulty > 0) CreateKnightKI(2984, 767, 400);
  CreateKnightKI(3121, 767, 400);
  CreateKnightKI(3071, 707, 400);
  // Burg mitte
  if(iDifficulty > 0) CreateBowmanKI(1512, 236, 400);
  CreateBowmanKI(1620, 296, 400);
  CreateBowmanKI(1309, 356, 400);
  CreateKnightKI(1408, 296, 400);
  CreateKnightKI(1364, 417, 400);
  if(iDifficulty > 0) CreateKnightKI(1561, 357, 400);
  if(iDifficulty > 1) CreateObject(MCLK, 1493, 417, -1)->SetAI(0, 500);
  // Ritter in der Höhle
  if(iDifficulty > 0) CreateKnightKI(1440, 809, 400);
  // Zauberclonk auf der Himmelsinsel
  CreateObject(MCLK, 919, 610, -1)->SetAI(0, 500);
  // Burg unten links
  if(iDifficulty > 1) CreateBowmanKI(682, 667, 400);
  CreateKnightKI(648, 788, 400);
  CreateKnightKI(600, 728, 400);
  // Burg links
  CreateBowmanKI(963, 317, 400);
  if(iDifficulty > 0) CreateKnightKI(835, 377, 400);
  CreateKnightKI(716, 438, 400);
  // Feldlager links
  if(iDifficulty > 1) CreateBowmanKI(226, 285, 400);
  CreateKnightKI(196, 337, 400);
  CreateKnightKI(65, 339, 400);

  // Pilze
  for(var i=0; i<5*(3-iDifficulty); i++)
    PlaceVegetation(MUSH, 0,0,LandscapeWidth(),LandscapeHeight(), -1);

  // Kein ScriptCounter mehr
  ScriptGo(false);
}

private func CreateGameObjects()
{
  if(GameObjectsCreated) return();
  GameObjectsCreated = true;

  // Spielziele und -regeln, Umweltobjekte
  CreateObject(_GIC, 10,10,-1); // Ziel: Goldstatuenbau
  CreateObject(SURR, 10,10,-1); // Aufgeben erlaubt
  CreateObject(CNMT, 10,10,-1); // Baumaterial
  CreateObject(TKNW, 10,10,-1); // Teamkonto
  CreateObject(TACC, 10,10,-1); // Team-Baupläne
  CreateObject(_ETG, 10,10,-1); // Löscher
  CreateObject(TIME, 10,10,-1); // Tag/Nacht
  CreateObject(EGLN, 10,10,-1); // Glitzernde Materialien
  for(var i=0; i<5; i++)
    CreateObject(STRS, 10,10,-1); // Sterne

  // Helligkeit setzen (ganz dunkel)
  SetFadeLightness(0, 2);

  // Es ist Nacht (zu Beginn)
  FindObject(TIME)->SetTime(50);

  // Objekte im Erdreich
  for(var i=0; i<15; i++)
  {
    PlaceInMaterial(LOAM, Material("Earth"), 50);
    PlaceInMaterial(GOLD, Material("Earth"), 50);
    PlaceInMaterial(ROCK, Material("Earth"), 50);
    PlaceInMaterial(ROCK, Material("Earth"), 50);
    if(Random(2)) PlaceInMaterial(FLNT, Material("Earth"), 50);
  }             

  // Bäume
  for(var i=0; i<10; i++)
    PlaceVegetation(TRE1, 0,0,LandscapeWidth(),LandscapeHeight(), -1);
  for(var i=0; i<20; i++)
    PlaceVegetation(TRE2, 0,0,LandscapeWidth(),LandscapeHeight(), -1);
  for(var i=0; i<14; i++)
    PlaceVegetation(TRE3, 0,0,LandscapeWidth(),LandscapeHeight(), -1);

  // Bäume hinter Burgteile
  ResortObjects("ResortPlantsProc", C4D_StaticBack);

  // ScriptCounter starten
  ScriptGo(true);
}

protected func ResortPlantsProc(object obj1, object obj2)
{
  // Pilze nach vorne
  if(GetID(obj1) == MUSH) return(-1);
  if(GetID(obj2) == MUSH) return(1);
  // Bäume nach hinten...
  if(obj2->~IsTree()) return(-1);
}

// Einen KI-Bogenschützen erzeugen
global func CreateBowmanKI(int iX, int iY, int iRange)
{
  var pKnight = CreateObject(KNIG, AbsX(iX), AbsY(iY), -1);
  pKnight->SetAI(0, iRange);
  // Bogen und Pfeile
  pKnight->CreateContents(BOW1);
  pKnight->CreateContents(ARWP);
  if(!Random(3)) pKnight->CreateContents(FARP);
  if(!Random(4)) pKnight->CreateContents(XARP);
  return(pKnight);
}

// Einen KI-Ritter erzeugen
global func CreateKnightKI(int iX, int iY, int iRange)
{
  var pKnight = CreateObject(KNIG, AbsX(iX), AbsY(iY), -1);
  pKnight->SetAI(0, iRange);
  return(pKnight);
}

