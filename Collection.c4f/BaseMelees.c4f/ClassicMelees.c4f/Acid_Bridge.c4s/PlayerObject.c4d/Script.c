/*-- Spielerobjekt --*/

#strict

local iSpieler;

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
 var x; if (GetPlayerTeam(iSpieler) == 1) x = 150; else x = 2350;
 while (Contents()) Exit(Contents(), x-GetX(), 450-GetY());

 SetCursor(iSpieler, GetCrew(iSpieler));
 CreateContents(FLAG,GetCursor(iSpieler));

 // Bei einem Relaunch gibt es noch ne Base im Sack dazu.
 if (GetAction() eq "Relaunch")
 {
	Sparkle();
	return(RemoveObject());
 }
 // Alle Spieler durchgehen, falls dieser der Erste in seinem Team ist, Hütte erschaffen.
 var i = GetPlayerCount();
 while (i--)
  if (GetPlayerByIndex(i) != iSpieler && GetPlayerTeam(GetPlayerByIndex(i)) == GetPlayerTeam(iSpieler))
   return(RemoveObject());

 if (GetPlayerTeam(iSpieler) == 2) CreateObject (HUT3,1100,430,-1);
  else CreateObject (HUT3,-1100,430,-1);
 return(RemoveObject());
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
