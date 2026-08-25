/*-- Feldlager --*/

#strict

// Die Spielernummern werden in Global(0 bis 3) gespeichert.
// Global(0): Spieler in der Burg
// Global(1 und 2): Spieler im Lager

static GameObj, playerplace;

static const PlaceCount = 3;
static Place;

protected func Initialize()
{  
  Place = [[1104,195], [123,386], [73,335]];
  Global(0)=Global(1)=Global(2)=-1;
  // Playlist setzen
  SetPlayList("Morning Dawn.mid;Territory.ogg;Camelot.mid");
  // Fahrstuhl "reparieren"
  var pObj = FindObject(CPEC);
  if(pObj) LocalN("RangeTop", pObj) = GetY(pObj);
}

protected func Script1()
{
  // Alleine spielen geht nicht...
  if(GetPlayerCount()==1)
    return(1, Log("$NotAlone$"), GameOver());
  // Keine Spielerbeitritte mehr
  SetMaxPlayer(0);
  // Keiner in der Burg? -> Einen dorthin setzen
  if(Global(0) == -1)
  { 
    // Anm.: Bei leerer Burg müssen Global(1) und Global(2) belegt sein!
    Global(0) = Global(2); 
    Global(2) = -1;
  }
  // Global(1) hat Vorrang gegenüber Global(2)
  if(Global(1) == -1)
  {
    Global(1) = Global(2);
    Global(2) = -1;
  }
  // Spieler an ihre Plätze setzen
  for(var i=0; i<3; i++)
    if(Global(i)!=-1)
      SetPlayer(Global(i), Place[i][0], Place[i][1]);
  // Spieler miteinander verfeinden
  SetHostility(Global(0), Global(1), 1, 1);
  SetHostility(Global(0), Global(2), 1, 1);
  SetHostility(Global(1), Global(0), 1, 1);
  SetHostility(Global(2), Global(0), 1, 1);
  SetHostility(Global(1), Global(2), 0, 1);
  SetHostility(Global(2), Global(1), 0, 1);
  // Burg gehört Spieler Global(0)
  SetObjOwner(Global(0), 920,0, 1230,230);
  // Lager + Sägewerk gehört Spieler Global(1)
  SetObjOwner(Global(1), 0,180, 315,415);
  SetOwner(Global(1), FindObject(SAWM));
  // Scriptcounter anhalten
  ScriptGo(0);
}

protected func InitializePlayer(int iPlr)
{
  // Spielobjekte erzeugen
  CreateGameObjects();

  var i, n;
  while (++i < 50)
  {
    n = Random(PlaceCount);
    if ((playerplace >> n) & 1) continue;
    playerplace = playerplace | (1 << n);
    Global(n) = iPlr;
    break;
  }
}

private func SetPlayer(plr, px, py)
{
  var obj;
  while (obj = FindObjectOwner(0, plr, 0,0,0,0, 0,0,0, obj))
    SetPosition(px+Random(25)-12, py, obj);
}

private func SetObjOwner(iPlr, iX, iY, iX2, iY2)
{
  var pObj;
  while(pObj = FindObject(0, iX, iY, iX2 - iX, iY2 - iY, 0,0,0,0, pObj))
    if(GetOwner(pObj) == -1)
      if(!(GetCategory(pObj) & C4D_Living))
        SetOwner(iPlr, pObj);
}

private func CreateGameObjects()
{
  // Spielobjekte nur einmal erzeugen
  if(GameObj) return(0);
  GameObj = 1;
        
  // Erdreichobjekte und Nester
  PlaceObjects(LOAM,20,"Earth");
  PlaceObjects(FLNT,20/5,"Earth");
  PlaceObjects(GOLD,20*3/5,"Earth");
  PlaceObjects(ROCK,20*4/5,"Earth");
  PlaceObjects(SKUL,2,"Earth");
  PlaceObjects(ZAPN,2,"Earth");
  PlaceObjects(MEGG,1,"Earth");
  PlaceObjects(FMEG,1,"Earth");
  PlaceObjects(FISH,4,"Water",,,,,,true);

  // Tiere
  for(var i=0; i<3; i++)
    PlaceAnimal(BIRD);

  // Bäume erzeugen
  for(var i=0; i<10; i++)
  {
    PlaceVegetation(TRE1, 0,0,LandscapeWidth(), LandscapeHeight(), -1);
    PlaceVegetation(TRE2, 0,0,LandscapeWidth(), LandscapeHeight(), -1);
    PlaceVegetation(TRE3, 0,0,LandscapeWidth(), LandscapeHeight(), -1);
    PlaceVegetation(TRE4, 0,0,LandscapeWidth(), LandscapeHeight(), -1);
  }

  // Bäume hinter Burgteile
  ResortObjects("ResortPlantsProc", C4D_StaticBack);

  // Ziel: Melee Teamwork
  CreateObject(MEL2, 10,10,-1);
  
  // Regeln
  CreateObject(CNMT, 10,10,-1); // Baumaterial
  CreateObject(SURR, 10,10,-1); // Aufgeben
  CreateObject(KILC, 10,10,-1); // Verlust Captain
  CreateObject(FGRV, 10,10,-1); // Flagge abnehmbar
  CreateObject(NMTT, 10,10,-1); // Kein Metall vor Gebäuden
  
  // Umweltobjekte
  CreateObject(BRDS, 10,10,-1); // Vogelgesang
  CreateObject(EGLN, 10,10,-1); // Glitzernde Materialien
  CreateObject(CLFS, 10,10,-1); // Bunte Fische

  // Scriptcounter starten
  ScriptGo(1);  
  
  return(1);
}

protected func ResortPlantsProc(object obj1, object obj2)
{
  // Bäume nach hinten...
  if(obj2->~IsTree()) return(-1);
}

