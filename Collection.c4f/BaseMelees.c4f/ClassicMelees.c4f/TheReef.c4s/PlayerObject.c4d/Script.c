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
 //SetFoW (1, player);
}

private func DoPlrLaunch()
{
 // Sound abspielen
 Sound("Ding", true);

 // Den Spieler bei seiner Position rauswerfen
 var x; if (GetPlayerTeam(iSpieler) == 1) x = 300; else x = 1870;

 // U-Boot erzeugen
 var pSub = CreateObject (SUB1,x-GetX(),445-GetY(),iSpieler);

 while (Contents())
	{
	var pClonk = Contents();
	SetPlrViewRange(120,pClonk);
	Enter(pSub,pClonk);
	}

 SetCursor(iSpieler, GetCrew(iSpieler)); 

 // Bei einem Relaunch ist es jetzt fertig.
 if (GetAction() eq "Relaunch")
 {
	Sparkle();
	return(RemoveObject());
 }
 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist, U-Boot erschaffen und Gebäude färben.
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iSpieler && GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iSpieler))
   return(RemoveObject());

 if (GetPlayerTeam(iSpieler) == 2) SetCastleColor(iSpieler, LandscapeWidth()/2, 20, LandscapeWidth()/2, LandscapeHeight()-20);
  else SetCastleColor(iSpieler, 0, 20, LandscapeWidth()/2, LandscapeHeight()-20);
 return(RemoveObject());
}

public func SetCastleColor(iPlayer, iPosx, iPosy, iWidth, iHeight)
{
 // Alles umfärben.
 for ( var obj in FindObjects( Find_InRect( iPosx-GetX(), iPosy-GetY(), iWidth, iHeight ), Find_Or( Find_OCF(OCF_Entrance), Find_ID(FLAG), Find_ID(BK5P), Find_ID(ANVL) ) ) )
 SetOwner(iPlayer, obj);
 for (var pObj in FindObjects(Find_InRect(iPosx-GetX(), iPosy-GetY(), iWidth, iHeight),Find_ID(BK5P))) pObj->Initialize();
 for (var pObj in FindObjects(Find_InRect(iPosx-GetX(), iPosy-GetY(), iWidth, iHeight),Find_ID(CTW0))) Enter(pObj,CreateObject(CTW7));
 /*if (!iPosx) CreateObject(_BIO,63-GetX(),718-GetY(),iPlayer);
 else CreateObject(_BIO,1870-GetX(),839-GetY(),iPlayer);*/

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
 if (GetActTime() >= 250) return(DoPlrLaunch());
 PlrMessage("$MsgRelaunchTime$", iSpieler, (250-GetActTime()) / 25 / 60, (250-GetActTime()) / 25 % 60);
}

/* Sonstiges */

protected func RejectCollect() { return(1); } // Damit man nichts in das Objekt ablegen kann
