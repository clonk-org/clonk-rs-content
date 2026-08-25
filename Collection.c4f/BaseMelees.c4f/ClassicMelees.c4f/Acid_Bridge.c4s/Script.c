/*-- Acid Bridge --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 0;
static relaunch;

func Initialize() 
{
	relaunch = [];
	Schedule("SetMaxPlayer(0)",100);	
	var DrainX = [LandscapeWidth()/2,LandscapeWidth()/2-350,LandscapeWidth()/2+350];
	for (var i=2;i>=0;i--) CreateObject(DRAI,DrainX[i],679);
	PlaceObjects(WOOD, 10, "FlyAshes", 0, 0, LandscapeWidth()/2, LandscapeHeight());
	PlaceObjects(WOOD, 10, "FlyAshes", LandscapeWidth()/2, 0, LandscapeWidth()/2, LandscapeHeight());
	ScriptGo(1);
	goto(1); 
	return(1);
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  SetFoW(1,player);    
  relaunch[player] = MaxRelaunch;
  // Den Spieler ausrüsten und platzieren
  CreateObject(_PLO, LandscapeWidth() / 2, 50, player)->InitPlayer(player,iTeam);
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

protected func Script100()
{
  // Mehr Spieler brauchen mehr Waffen
  for (var i=0; i<GetPlayerCount(); ++i)
    {
    // Typ bestimmen
    var id = getSpawnType();
    // Platz im Erdreich finden
    var x = 10 + Random(LandscapeWidth() - 20);
    var h = GetHorizonHeight(x);
    var y = h + 10 + Random(LandscapeHeight() - h - 20);
    if (GetMaterial(x, y) != Material("Earth")) continue;
    // Objekt per verzögertem Effekt erzeugen
    var effect = AddEffect("SpawnSparkle", 0, 1, 5, 0, 0);
    EffectVar(0, 0, effect) = id; EffectVar(1, 0, effect) = x; EffectVar(2, 0, effect) = y;
    }
}

protected func Script150()
{
  goto(100);
}

private func getSpawnType()
{
  var id = [CRYS,WOOD,ROCK,METL];
  return (id[Random(GetLength(id))]);
}  

private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return(y);
}

global func FxSpawnSparkleTimer(obj, effect, time)
{
  // Variablen auswerten
  var id = EffectVar(0, 0, effect); var x = EffectVar(1, 0, effect); var y = EffectVar(2, 0, effect);
  // Funken erzeugen
  CreateParticle("NoGravSpark", x + RandomX(-6,+6), y + RandomX(-6,+6), 0, -3, RandomX(30,50), RGB(64,64,255));
  // Objekt erzeugen
  if ((time >= 100) && !EffectVar(3, 0, effect)) 
    EffectVar(3, 0, effect) = CreateObject(id, x, y);
  // Fertig
  if (time >= 150) return(-1);
}