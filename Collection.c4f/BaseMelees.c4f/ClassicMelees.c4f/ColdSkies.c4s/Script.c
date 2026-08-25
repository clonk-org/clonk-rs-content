/*-- Cold Skies --*/

#strict

static g_iSpawnCount;
static Xpos;
static Ypos;
static TeamID;

func Initialize() 
{	
	VegiSetUp();
	ScriptGo(1);
	goto(1); 
	return(1);
}

protected func InitializePlayer(iPlr)
{
  Equip(iPlr,FindObject2(Find_ID(MCLK),Find_Owner(iPlr)));
  return(1);
}

protected func VegiSetUp()
{
	var Vegis =[BONE, SKUL, IDOL, TRB3, TRB2, TRB1, TRB4];
	for (var i=0; i<5; i++)
		{
		for (var j=0; j<GetLength(Vegis); j++)
			{
			var iX = RandomX(120,2360);
			var iY = GetHorizonHeight(iX)+5;
			CreateObject(Vegis[j], iX, iY, -1);
			}
		}
	return(1);
}

global func Equip(player,pObj){
  CreateContents(FLAG,pObj);
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
		GetCrew(k,0)->SetPosition(Xpos[Rnd],Ypos[Rnd]);
		Xpos[Rnd] = Xpos[iLength-1];
		Ypos[Rnd] = Ypos[iLength-1];
		SetLength(Xpos, iLength-1);
		SetLength(Ypos, iLength-1);
  		}
}

global func DoFaceTeams()
{
	CreateObject (BRDG,36,LandscapeHeight()/2+10,-1)->Lock();
	CreateObject (BRDG,109,LandscapeHeight()/2+10,-1)->Lock();
	CreateObject (CST3,73,LandscapeHeight()/2+9,-1);
	CreateObject (BRDG,LandscapeWidth()-36,LandscapeHeight()/2+10,-1)->Lock();
	CreateObject (BRDG,LandscapeWidth()-109,LandscapeHeight()/2+10,-1)->Lock();
	CreateObject (CST3,LandscapeWidth()-73,LandscapeHeight()/2+9,-1);

	for (var k = 0; k < GetPlayerCount(); k++)
		{
		if (GetPlayerTeam(k) == TeamID[0]) GetCrew(k,0)->SetPosition(60,LandscapeHeight()/2);
		if (GetPlayerTeam(k) == TeamID[1]) GetCrew(k,0)->SetPosition(LandscapeWidth()-60,LandscapeHeight()/2);
  		}
	return(goto(10));
}

global func GetValidTeams()
{
	TeamID = [];
	for (var j = 0; j < GetTeamCount(); j++)
		{
		for (var m = 0; m < GetPlayerCount(); m++)
			{
			if (GetPlayerTeam(m)==GetTeamByIndex(j))
				{
				TeamID[GetLength(TeamID)] = GetTeamByIndex(j);
				m = GetPlayerCount();
				}
			}
		}
	return(GetLength(TeamID));
}

protected func Script1()
{
RandomStartPositions();
if (GetValidTeams() == 2) DoFaceTeams();
}

protected func Script99()
{
	return(SetMaxPlayer());
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
    if (GetMaterial(x, y) != Material("Snow")) continue;
    // Objekt per verzögertem Effekt erzeugen
    var effect = AddEffect("SpawnSparkle", 0, 1, 5, 0, 0);
    EffectVar(0, 0, effect) = id; EffectVar(1, 0, effect) = x; EffectVar(2, 0, effect) = y;
    }
    // Spawns mitzählen
    g_iSpawnCount++;    
}

protected func Script150()
{
  goto(100);
}

private func getSpawnType()
{
  var id = [WOOD, METL, CRYS, _SKL, ROCK, SCRL];
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