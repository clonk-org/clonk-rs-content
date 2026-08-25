/*-- Geld-Versorgung --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

func CheckMoney() // alle 400 Frames
{
// Alle Teams durchgehen und 5 Clunker geben.
	for(var i = 0; i < GetTeamCount(); i++)
		{
		// Alle Spieler durchgehen und Teammitglied finden
		for(var j = 0; j < GetPlayerCount(); j++)
			{
			var iSpieler = GetPlayerByIndex(j);
			if (GetTeamByIndex(i)==GetPlayerTeam(iSpieler)) break;
			}
		SetWealth (iSpieler, GetWealth(iSpieler)+5);
		}
}
