/*-- Spielerobjekt --*/

#strict

local iSpieler;
static Canonballs;
static pWeapon;
static pAmmo;

/* Spielstart */

protected func InitPlayer(int player, int iTeam)
{
 iSpieler = player;
 for (var i; GetCrew(iSpieler, i); i++) Enter(this(), GetCrew(iSpieler, i));
 DoPlrLaunch();
 SetFoW (1, player);
}

private func DoPlrLaunch()
{
 // Sound abspielen
 Sound("Ding", true);

 // Den Spieler bei seiner Position rauswerfen
 var x; if (GetPlayerTeam(iSpieler) == 1) x = 100; else x = 1830;
 while (Contents()) Exit(Contents(), x-GetX(), 100-GetY());

  SetCursor(iSpieler, GetCrew(iSpieler));

  Canonballs = [_CBL,_CCL];
  pWeapon    = [AXE1,BOW1,BOW1,GUNP];
  pAmmo      = [SFLN,XARP,CARP,_XCB];

  if (Random(2))
	{
	var j = Random(2);
	if( FindObject2( Find_ID(KING), Find_Hostile(iSpieler) ) ) j=RandomX(2,3);
	if( FindObject2( Find_ID(_SSR), Find_Hostile(iSpieler) ) ) j=RandomX(2,3);
	CreateContents(pWeapon[j],GetCursor(iSpieler));
	CreateContents(pAmmo[j],GetCursor(iSpieler));
	}
  else
  {
  CreateContents(Canonballs[Random(2)],GetCursor(iSpieler));
  CreateContents(GUNP,GetCursor(iSpieler));
  CreateContents(METL,GetCursor(iSpieler));
  }

 // Bei einem Relaunch ist es jetzt fertig.
 if (GetAction() eq "Relaunch")
 {
	Sparkle();
	return(RemoveObject());
 }
 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist, wird die Burg nach ihm gefärbt.
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iSpieler && GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iSpieler))
   return(RemoveObject());

 if (GetPlayerTeam(iSpieler) == 2) SetCastleColor(iSpieler, LandscapeWidth()/2, 5, LandscapeWidth()/2, LandscapeHeight()-5);
  else SetCastleColor(iSpieler, 0, 5, LandscapeWidth()/2, LandscapeHeight()-5);

	CreateObject(LANC,0,0,iSpieler)->Activate(GetCursor(iSpieler));
	CreateObject(SHIE,0,0,iSpieler)->Activate(GetCursor(iSpieler));
	GetCursor(iSpieler)->SetArmored(1, ARMR);
	var horse = CreateObject(HRSK,x-GetX(), 100-GetY(),iSpieler);
	ObjectSetAction(GetCursor(iSpieler),"Ride",horse);

 return(RemoveObject());
}

public func SetCastleColor(iPlayer, iPosx, iPosy, iWidth, iHeight)
{
 // Alles umfärben.
 for (var obj in FindObjects(Find_InRect(iPosx-GetX(), iPosy-GetY(), iWidth, iHeight), Find_Not(Find_OCF(OCF_CrewMember))))
 SetOwner(iPlayer, obj);

 // Fertig!
 return(1);
}

/* Relaunch */

protected func RelaunchPlayer(int player)
{
 iSpieler = player;
 for (var i; GetCrew(iSpieler, i); i++) Enter(this(), GetCrew(iSpieler, i));
 SetAction("Relaunch");
}

private func Execute()
{
 if (!Contents()) return(RemoveObject());
 if (GetActTime() >= 50) return(DoPlrLaunch());
 PlrMessage("$MsgRelaunchTime$", iSpieler, (50-GetActTime()) / 25 / 60, (50-GetActTime()) / 25 % 60);
}

/* Sonstiges */

protected func RejectCollect() { return(1); } // Damit man nichts in das Objekt ablegen kann
