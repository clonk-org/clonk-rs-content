/*-- The Reef --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 0;
static relaunch;

func Initialize() {
  relaunch = [];
  Schedule("SetMaxPlayer(0)",100);
  VegiSetUp();
  return(1);
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  SetFoW(1,player);    
  relaunch[player] = MaxRelaunch;
  // Den Spieler ausrüsten und platzieren
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player,iTeam);
  for (var pObj in FindObjects(Find_ID(IDOL))) pObj->Initialize();
  for (var pObj in FindObjects(Find_ID(GIDL))) pObj->Initialize();
  for(var y=0; y<LandscapeHeight(); (y+=590) && (x=0))
	for(var x=0; x<LandscapeWidth(); x+=470)
		ObjectSetAction(CreateObject(_CSN,x,y,-1),Format("Wait%d",Random(4)));
  return(1);
}

public func RelaunchPlayer(int iPlr){
if (relaunch[iPlr] == 0)
	{
	return(1);
	}
  relaunch[iPlr] --;

  var pObj = CreateObject(ACLK,0,0,iPlr);
  SetOwner(iPlr,pObj);
  MakeCrewMember(pObj, iPlr);
  pObj->DoEnergy(100);
  SetCursor(iPlr, pObj);
  CreateObject(_PLO, LandscapeWidth() / 2, 50, iPlr)->RelaunchPlayer(iPlr);
}

protected func VegiSetUp()
{
  
  Message("$Processing$");
  var VegisOnGround =[BONE,SHEL,SKUL,SWD2,SWD1];				// IDs der möglichen Vegitation (doppelt so viele Lianen wie Stalaktiten)
  var VegisFactor = [10,5,5,50,50];						// Anzahl für Häufigkeit
	for (var j=0; j<GetLength(VegisOnGround); j++)
		PlaceObjects(VegisOnGround[j], VegisFactor[j], "GBackLiquid", 0, 0, LandscapeWidth(), LandscapeHeight(), 0, 1);  // Objekte am Meeresboden plazieren

  var Vegis =[_STA];								// IDs der möglichen Vegitation (doppelt so viele Lianen wie Stalaktiten)
  for ( var k=0; k<=LandscapeHeight(); k+=LandscapeHeight()/10 )		// Obere Schleife: Teilt die Lanschaftshöhe in 10 gleich hohe Zeilen
	for (var i=0; i<LandscapeWidth()/300; i++)				// Mittlere Schleife: Platziert in jeder Zeile LandscapeWidth()/150 Objekte
		{
		for (var j=0; j<GetLength(Vegis); j++)				// Untere Schleife: Für jede ID im Array Vegis[] wird i mal eine mögliche Postion gesucht
			{
			var iX = RandomX(LandscapeWidth()+1);			// Die Zufällige X-Position
			var iY = GetHorizonPos(iX,k,k+LandscapeHeight()/10);	// Eine Y-Position wird gesucht. Begonnen wird mit dem oberen Rand der Zeile und endet mit dem unteren Rand der Zeile
			if (iY) CreateObject(Vegis[j], iX, iY, -1);		// Wenn GetHorizonPos() 0 zurückgibt nix plazieren. Gilt auch für die oberen beiden Sätze.
			}
		}
  //Schedule("MineReef(100)", 100);
	return(1);
}

private func GetHorizonPos(x,LimUp,LimLow)	// Funktion sucht mögliche Y-Position (X-Pos, oberer Zeilenrand, unterer Zeilenrand) UNTER WASSER!!!!!!!!!!!
{
  var y = LimUp;				// Variable für Y Position; Startwert ist oberer Zeilenrand
  while (y < LimLow && !GBackSolid(x, y))       // Solange Y nach unten schieben bis eine Insel gefunden wurde
	y += 10;
	
  while (y < LimLow && GBackSolid(x, y))	// Dann langsam nach unten schieben bis der Rand der Insel gefunden wurde
	++y;

  if (y >= LimLow || y <= LimUp) return(0);	// Sind wir schon aus der Zeile gerutscht? Dann 0 zurückgeben.
  if (GBackSolid(x, y+64)) return(0);		// Ist unter der insel auch genug Platz? Wenn nicht 0 zurückgeben.

  return(y);					// Sonst Y-Position zurückgeben
}

private func MineReef(int iAmount)
{
  // Riff verminen (zeitverzögert)
  PlaceObjects(BJ5P, iAmount, "GBackLiquid", LandscapeWidth()/2-300, 0, 600, LandscapeHeight()-100, 0, 1);
  return(1);
}