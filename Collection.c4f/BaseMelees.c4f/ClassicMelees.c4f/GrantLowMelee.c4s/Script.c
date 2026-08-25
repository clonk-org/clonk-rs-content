/*-- Grand Low Melee --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 1;
static relaunch;
static xP;
static yP;
static Xpos;
static Ypos;

func Initialize() {
  relaunch = [];
  xP = [];
  yP = [];
  Schedule("SetMaxPlayer(0)",100);
  Schedule("RandomStartPositions()",2);
  SetGamma(RGB(0,0,0),RGB(68,68,83),RGB(147,147,173));
  for(var y=0; y<LandscapeHeight(); (y+=510) && (x=0))
	for(var x=0; x<LandscapeWidth(); x+=530)
		ObjectSetAction(CreateObject(_CSN,x,y,-1),Format("Wait%d",Random(4)));
  VegiSetUp();
  return(1);
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  SetTemperature (110);
  SetFoW(1,player);    
  relaunch[player] = MaxRelaunch;
  for(var pClnk;pClnk=FindObjectOwner(0,player,0,0,0,0,OCF_CrewMember(),0,0,pClnk);)
  pClnk->Initialize();
  return(1);
}
global func RandomStartPositions()
{
	Xpos = [];
	Ypos = [];
	for (var i = 0; i < GetPlayerCount(); i++)
		{
		Xpos[i] = GetX(GetCursor(i));
		Ypos[i] = GetY(GetCursor(i));
		}
	
	for (var k = 0; k < GetPlayerCount(); k++)
		{
		var iLength = GetLength(Xpos);
		var Rnd = Random(iLength);
		var c=GetCrewCount(0);
		var pHut;
		Var(0)=Xpos[Rnd]; Var(1)=Ypos[Rnd];

		if(FindConstructionSite(CST1, 0, 1))
		pHut = CreateConstruction(CST1, Var(0), Var(1), k, 100, 1);

		while (c--)
		GetCrew(k,c)->Enter(pHut);

		CreateContents(FLAG,pHut,1);
		CreateContents(CNKT,pHut,1);
		CreateContents(_TRC,pHut,2);

		xP[k] = Xpos[Rnd];
		yP[k] = Ypos[Rnd];
		Xpos[Rnd] = Xpos[iLength-1];
		Ypos[Rnd] = Ypos[iLength-1];
		SetLength(Xpos, iLength-1);
		SetLength(Ypos, iLength-1);
  		}
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