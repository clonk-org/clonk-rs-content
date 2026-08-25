/*-- Burning Skies --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 0;
static relaunch;

func Initialize() {
  relaunch = [];
  Schedule("SetMaxPlayer(0)",100);
  for (var pObj in FindObjects(Find_ID(1RKT))) pObj->Initialize();
  CreateObject(TACC);
  VegiSetUp();
  //ObjSetUp();
  PlaceObjects(METL,30,"Rock",0,0,LandscapeWidth()/2,LandscapeHeight());
  PlaceObjects(BARL,30,"Rock",0,0,LandscapeWidth()/2,LandscapeHeight());
  PlaceObjects(METL,30,"Rock",LandscapeWidth()/2,0,LandscapeWidth()/2,LandscapeHeight());
  PlaceObjects(BARL,30,"Rock",LandscapeWidth()/2,0,LandscapeWidth()/2,LandscapeHeight());
  return(1);
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  SetFoW(1,player);    
  relaunch[player] = MaxRelaunch;
  // Den Spieler ausrüsten und platzieren
  CreateObject(_PLO, LandscapeWidth() / 2, 0, player)->InitPlayer(player,iTeam);
  MessageWindow(GetDesc(0,1RKT), player,1RKT,GetName(0, 1RKT));
  return(1);
}

public func RelaunchPlayer(int iPlr){
if (relaunch[iPlr] == 0)
	{
	return(1);
	}
  relaunch[iPlr] --;

  var pObj = CreateObject(CLNK,0,0,iPlr);
  SetOwner(iPlr,pObj);
  MakeCrewMember(pObj, iPlr);
  pObj->DoEnergy(100);
  SetCursor(iPlr, pObj);
  CreateObject(_PLO, LandscapeWidth() / 2, 50, iPlr)->RelaunchPlayer(iPlr);
}

protected func VegiSetUp()
{
  var Vegis =[_STA];								// IDs der möglichen Vegitation (doppelt so viele Lianen wie Stalaktiten)

  for ( var k=0; k<=LandscapeHeight(); k+=LandscapeHeight()/10 )		// Obere Schleife: Teilt die Lanschaftshöhe in 10 gleich hohe Zeilen
	for (var i=0; i<LandscapeWidth()/50; i++)				// Mittlere Schleife: Platziert in jeder Zeile LandscapeWidth()/50 Objekte
		{
		for (var j=0; j<GetLength(Vegis); j++)				// Untere Schleife: Für jede ID im Array Vegis[] wird i mal eine mögliche Postion gesucht
			{
			var iX = RandomX(LandscapeWidth()+1);			// Die Zufällige X-Position
			var iY = GetHorizonPos(iX,k,k+LandscapeHeight()/10);	// Eine Y-Position wird gesucht. Begonnen wird mit dem oberen Rand der Zeile und endet mit dem unteren Rand der Zeile
			if (iY) CreateObject(Vegis[j], iX, iY, -1);		// Wenn GetHorizonPos() 0 zurückgibt nix plazieren. Gilt auch für die oberen beiden Sätze.
			}
		}
	return(1);
}

private func GetHorizonPos(x,LimUp,LimLow)	// Funktion sucht mögliche Y-Position (X-Pos, oberer Zeilenrand, unterer Zeilenrand)
{
  var y = LimUp;				// Variable für Y Position; Startwert ist oberer Zeilenrand
  while (y < LimLow && !GBackSemiSolid(x, y))   // Solange Y nach unten schieben bis eine Insel gefunden wurde
	y += 10;
	
  while (y < LimLow && GBackSemiSolid(x, y))	// Dann langsam nach unten schieben bis der Rand der Insel gefunden wurde
	++y;

  if (y >= LimLow || y <= LimUp) return(0);	// Sind wir schon aus der Zeile gerutscht? Dann 0 zurückgeben.
  if (GBackSemiSolid(x, y+64)) return(0);	// Ist unter der insel auch genug Platz? Wenn nicht 0 zurückgeben.

  return(y);					// Sonst Y-Position zurückgeben
}