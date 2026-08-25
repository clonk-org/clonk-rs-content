/*-- Spielerobjekt --*/

#strict

local iSpieler;

/* Spielstart */

protected func InitPlayer(int player, int iTeam)
{
 iSpieler = player;
 for (var i; GetCrew(iSpieler, i); i++) Enter(this(), GetCrew(iSpieler, i));
 DoPlrLaunch();
}

private func DoPlrLaunch()
{
 // Sound abspielen
 Sound("Ding", true);

 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist, Hütte und Turm suchen.
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iSpieler && GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iSpieler))
	{
	var pBase = FindObject2(Find_ID(CST1),Find_Allied(iSpieler),Find_Not(Find_Owner(NO_OWNER)));
	// Den Spieler bei seiner Position rauswerfen
	var x = GetX(pBase);
	var y = GetY(pBase);
	while (Contents()) Exit(Contents(), x-GetX(), y-GetY()+15);
	SetCursor(iSpieler, GetCrew(iSpieler));
	return(RemoveObject());
	}

var pHuts = FindObjects(Find_ID(CST1),Find_Owner(NO_OWNER));
var pBase = pHuts[Random(GetLength(pHuts))];
var pTower = pBase->FindObject(TWR2, 0, 0, -1, -1); 
SetOwner(iSpieler,pBase); 
SetOwner(iSpieler,pTower);
var pFlag = CreateContents(FLAG,pBase,1);
pFlag -> SetOwner(iSpieler);
// Den Spieler bei seiner Position rauswerfen
var x = GetX(pBase);
var y = GetY(pBase);
while (Contents()) Exit(Contents(), x-GetX(), y-GetY()+15);
SetCursor(iSpieler, GetCrew(iSpieler));

return(RemoveObject());
}

/* Sonstiges */

protected func RejectCollect() { return(1); } // Damit man nichts in das Objekt ablegen kann
