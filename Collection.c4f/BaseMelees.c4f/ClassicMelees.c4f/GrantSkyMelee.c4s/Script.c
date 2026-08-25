/*-- Grand Sky Melee --*/

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
  Schedule("RandomStartPositions()",1);
  return(1);
}

protected func InitializePlayer(int player, X, Y, foo, iTeam)
{
  SetFoW(1,player);    
  relaunch[player] = MaxRelaunch;
  return(1);
}

public func RelaunchPlayer(int iPlr){
if (relaunch[iPlr] == 0)
	{
	return(1);
	}
  relaunch[iPlr] --;

  var pObj = CreateObject(CLNK,xP[iPlr],yP[iPlr],iPlr);
  SetOwner(iPlr,pObj);
  MakeCrewMember(pObj, iPlr);
  pObj->DoEnergy(100);
  SetCursor(iPlr,pObj);
  // Den Spieler ausrüsten
  CreateContents(FLAG,pObj);
  CreateContents(CNKT,pObj);
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

		if(FindConstructionSite(HUT3, 0, 1))
		pHut = CreateConstruction(HUT3, Var(0), Var(1), k, 100, 1);

		while (c--)
		GetCrew(k,c)->Enter(pHut);

		CreateContents(FLAG,pHut,1);
		CreateContents(CNKT,pHut,1);

		xP[k] = Xpos[Rnd];
		yP[k] = Ypos[Rnd];
		Xpos[Rnd] = Xpos[iLength-1];
		Ypos[Rnd] = Ypos[iLength-1];
		SetLength(Xpos, iLength-1);
		SetLength(Ypos, iLength-1);
  		}
}