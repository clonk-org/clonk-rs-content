/*-- KillTheCaptain --*/

#strict

local fNoMoreChecks;

private func Execute()
{
  if(fNoMoreChecks) return;

  var eliminate_castle  = !CheckTeam(Position_Castle);
  var eliminate_village = !CheckTeam(Position_Village);

  if(eliminate_castle || eliminate_village)
    fNoMoreChecks = true;

  // Beide Captains tot?
  //  -> seltener Fall... alle eliminieren!
  if(eliminate_castle && eliminate_village)
  {
    Log("$Draw$");
    for(var p,i=0; i<GetPlayerCount(); i++)
      EliminatePlayer(p=GetPlayerByIndex(i));
    return;
  }

  // Wenn Team in der Burg keinen Captain besitzt
  //  -> Alle Spieler des Teams eliminieren
  if(eliminate_castle)
    for(var i=0; i<GetPlayerCount(); i++)
      if(Global(GetPlayerByIndex(i)) == Position_Castle)
        EliminatePlayer(GetPlayerByIndex(i));
      else
        PlayerMessage(GetPlayerByIndex(i), "@<c %x>$VillageVictory$</c>", 0, HSL(Random(256),255,100+Random(60)));

  // Wenn Team im Dorf keinen Captain besitzt
  //  -> Alle Spieler des Teams eliminieren
  if(eliminate_village)
    for(var i=0; i<GetPlayerCount(); i++)
      if(Global(GetPlayerByIndex(i)) == Position_Village)
        EliminatePlayer(GetPlayerByIndex(i));
      else
        PlayerMessage(GetPlayerByIndex(i), "@<c %x>$CastleVictory$</c>", 0, HSL(Random(256),255,100+Random(60)));

}

// Prüft, ob ein Spieler des Teams einen Captain besitzt
private func CheckTeam(int iTeam)
{
  var plr;
  for(var i=0; i<GetPlayerCount(); i++)
    if(Global(plr=GetPlayerByIndex(i))==iTeam)
      for(var j=0; j<GetCrewCount(plr); j++)
        if(IsCaptain(GetCrew(plr, j)))
          return true;
  return false;
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return true;
}
