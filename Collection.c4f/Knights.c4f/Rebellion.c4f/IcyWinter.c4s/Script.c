/*-- Eisiger Winter --*/

#strict

// Global(Spielernummer) = Spielerposition
static const Position_Left = 1,
             Position_Right = 2,
             Position_Castle = 3;

static GameObjectsCreated, iInitializedPlayers;

static pKing;

protected func Initialize()
{
  // Tolle Schlachtmusik
  Music("The Chant.ogg");
  SetPlayList("The Chant.ogg;Darkside.mid;Rush.ogg");
  // Noch kein bestimmter Spieler als König vorgesehen
  DesignatedThanathosPlayer = -1;
  // Objekte erzeugen
  CreateGameObjects();
}

protected func InitializePlayer(int iPlr, int iX, int iY, int was_ist_das, int iTeam)
{
  iInitializedPlayers++;
  // Scriptcounter starten
  ScriptGo(true);
  // Fog of War
  SetFoW(true, iPlr);
  // Team setzen
  Global(iPlr) = iTeam;
}

protected func RelaunchPlayer(int iPlr)
{
  // Letzer Angreifer eliminiert?
  if(Global(iPlr) != Position_Castle)
    if((GetTeamPlayerCount(Position_Left)+GetTeamPlayerCount(Position_Right)) <= 1)
    {
      DesignatedThanathosPlayer = iPlr;
      Script3420();
      SelectCrew(iPlr, pKing, true);
      goto(3421);
      return;
    }
  // Spieler entfernen
  iInitializedPlayers--;
  Global(iPlr) = 0;
}

public func GetTeamPlayerCount(int iTeam)
{
  var c;
  for(var i=0; i<GetPlayerCount(); i++)
    if(Global(GetPlayerByIndex(i)) == iTeam)
      c++;
  return c;
}

protected func Script1()
{
  // Haben schon alle Spieler ausgewählt?
  if(iInitializedPlayers < GetPlayerCount()) return(goto(1));
  // Helligkeit setzen (ganz dunkel)
  SetFadeLightness(0, 2);
  // Kein Spieler in der Burg?
  for(var i=0,p=-1; i<GetPlayerCount(); i++)
    if(Global(GetPlayerByIndex(i)) == Position_Castle)
      p = GetPlayerByIndex(i);
  if(p == -1)
  {
    var p = Random(GetPlayerCount());
    Global(p) = Position_Castle;
    Log("$EmptyCastle$", GetPlayerName(p));
  }
  // König erzeugen
  CreateObject(ICBL,,,-1)->SetAction("Freeze", pKing=CreateObject(KING, 659, 969, -1));
  pKing->SetName("Thanathos");
  pKing->CreateContents(SHIE)->Activate(pKing);
  CLFS->Colorize(pKing);
  // Film starten
  CreateObject(_F00,10,10,-1)->StartFilm();
  CreateObject(_F01,10,10,-1)->StartFilm();
  CreateObject(_F02,10,10,-1)->StartFilm();
  CreateObject(_F03,10,10,-1)->StartFilm();
  // Verfeindung setzen
  for(var i=0; i<GetPlayerCount(); i++)
    for(var j=0; j<GetPlayerCount(); j++)
    {
      SetHostility(i, j, Sgn(Global(GetPlayerByIndex(i))-3) != Sgn(Global(GetPlayerByIndex(j))-3), true);
      SetHostility(j, i, Sgn(Global(GetPlayerByIndex(i))-3) != Sgn(Global(GetPlayerByIndex(j))-3), true);
    }
  // Einfaden
  Fade(0, 128, 2, 1, 150);
  // Scriptcounter anhalten (wird wieder von FilmCastle gestartet)
  ScriptGo(false);
}

protected func Script2()
{
  // Zeitangabe erzeugen
  CreateObject(_TIM,,,-1);
}

/*   
protected func Script50()
{
  // Während dem Testen bin ich ungeduldig!!!
  goto(3300);
}
// */

protected func Script3400()
{
  // Nachricht an Spieler
  Message(Format("<c %x>< %s ></c>|<c c8c8c8>$melting$", GetColorDw(pKing), GetName(pKing)));
}

static DesignatedThanathosPlayer;
protected func Script3420()
{
  // Thanathos auftauen
  if(FindObject(ICBL)) FindObject(ICBL)->Destruct();
  // Thanathos spielbar machen
  var p=-1,i,r;
  if((p=DesignatedThanathosPlayer) == -1)
  {
    for(i=0; i<50; i++)
      if(Global(GetPlayerByIndex(r=Random(GetPlayerCount()))) != Position_Castle)
        { p=GetPlayerByIndex(r); break; }
    // Noch kein Spieler?
    if(p == -1)
      for(i=0; i<GetPlayerCount(); i++)
        if(Global(GetPlayerByIndex(i)) != Position_Castle)
          { p=GetPlayerByIndex(r); break; }
  }
  // Immer noch kein Spieler? Dann ist Thanathos nicht spielbar.
  if(p != -1)
  {
    pKing->CreateContents(ANML);
    pKing->CreateContents(SWOR);
    var clr = GetColorDw(pKing);
    SetOwner(p, pKing);
    MakeCrewMember(pKing, p);
    SetColorDw(clr, pKing);
    Log("$tlog1$", GetPlayerName(p));
    Log("$tlog2$");
  }
}

protected func Script3423()
{
  // Eisblock ist noch da?
  if(FindObject(ICBL)) return(goto(3423));
  // Thanathos sagt was (als Message für alle Spieler)
  Message(Format("<c %x>< %s ></c>|<c c8c8c8>$free$", GetColorDw(pKing), GetName(pKing)));
  // ScriptCounter wird nicht mehr gebraucht
  ScriptGo(false);
}

// Gibt das Vorzeichen zurück (Signumfunktion)
global func Sgn(int number)
{
  if(!number) return(0);
  return(number/Abs(number));
}

private func CreateGameObjects()
{
  if(GameObjectsCreated) return();
  GameObjectsCreated = true;

  // Spielziele und -regeln, Umweltobjekte
  CreateObject(_STK, 10,10,-1);
  CreateObject(SURR, 10,10,-1);
  CreateObject(CNMT, 10,10,-1);
  CreateObject(STSN, 10,10,-1);
  CreateObject(TKNW, 10,10,-1);
  CreateObject(TACC, 10,10,-1);
  CreateObject(SCAH, 10,10,-1);
  CreateObject(NJ6C, 10,10,-1);
  CreateObject(RSNB, 10,10,-1);
  CreateObject(TIME, 10,10,-1);
  CreateObject(EGLN, 10,10,-1);
  for(var i=0; i<5; i++)
    CreateObject(STRS, 10,10,-1);

  // Objekte im Erdreich
  for(var i=0; i<10; i++)
  {
    PlaceInMaterial(LOAM, Material("Earth"), 50);
    PlaceInMaterial(GOLD, Material("Earth"), 50);
    PlaceInMaterial(ROCK, Material("Earth"), 50);
  }

  // Bäume
  for(var i=0; i<5; i++)
    PlaceVegetation(TRE1, 0,675,LandscapeWidth(),LandscapeHeight()-675, -1);
  for(var i=0; i<10; i++)
    PlaceVegetation(TRE2, 0,675,LandscapeWidth(),LandscapeHeight()-675, -1);
  for(var i=0; i<7; i++)
    PlaceVegetation(TRE3, 0,675,LandscapeWidth(),LandscapeHeight()-675, -1);

  // Bäume hinter Burgteile
  ResortObjects("ResortPlantsProc", C4D_StaticBack);

  // ScriptCounter starten
  ScriptGo(true);
}

protected func ResortPlantsProc(object obj1, object obj2)
{
  // Bäume nach hinten...
  if(obj2->~IsTree()) return(-1);
}

// Sofortiges Spielerbeitrittsverbot!
public func NJ6C_GetTimeSpan()
{
  return(5);
}

