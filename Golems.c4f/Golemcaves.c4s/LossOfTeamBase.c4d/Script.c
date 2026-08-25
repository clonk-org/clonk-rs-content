/*-- LossOfTeamBase --*/

#strict

private func Execute() {
  var team_count = 2;
  for (var team = 0; team < team_count; team++)
    if (!BaseCount(team)) EliminateTeam(team);
}

private func BaseCount(team, team_count) {
  if (!team_count) team_count = 2;
  var result = 0;

  // Alle Spieler überprüfen
  var plr_num;
  for (var plr_cnt = GetPlayerCount(); plr_cnt; plr_num++) {
    if (GetPlayerName(plr_num)) {
      plr_cnt--;
      if (plr_num % team_count != team) continue;

      if (FindBase(plr_num)) result++;
    }
  }
  
  return(result);
}

private func EliminateTeam(team, team_count) {
  if (!team_count) team_count = 2;
  
  // Alle Spieler überprüfen
  var plr_num;
  for (var plr_cnt = GetPlayerCount(); plr_cnt; plr_num++) {
    if (GetPlayerName(plr_num)) {
      plr_cnt--;
      if (plr_num % team_count != team) continue;
	
      EliminatePlayer(plr_num);
    }
  }
}
      
protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}
