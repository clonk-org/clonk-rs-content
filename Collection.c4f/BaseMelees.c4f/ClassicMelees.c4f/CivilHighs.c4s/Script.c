/*-- CivilHigh --*/

#strict

//Relaunch anzahl:
static const MaxRelaunch = 1;
static relaunch;

func Initialize() {
  relaunch = [];
  Schedule("SetMaxPlayer(0)",100);
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