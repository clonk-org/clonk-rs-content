/*-- Benötigte Kills einstellen --*/

#strict 2

protected func Activate(iPlr)
{
  var fRelaunched;
  for(var iDeaths in aDeaths)
  {
    if(iDeaths) fRelaunched = 1;
  }
  
  if(iPlr != GetPlayerByIndex() || fRelaunched || GetActTime()>5400)
  {
    PlayerMessage(iPlr,"$showF$",0,g_MaxKills);
    return 0;
  }
  
  CreateMenu(RCHS,GetCursor(iPlr), this,0,0,0,0,1);
  AddMenuItem("$IncF$","Relaunchs(+1)", RCHS, GetCursor(iPlr), 0, iPlr, " ", 2, 1);
  AddMenuItem("$DecF$", "Relaunchs(-1)", RCHS, GetCursor(iPlr), 0, 0, " ", 2, 2);
}

global func Relaunchs(val)
{
  g_MaxKills = BoundBy(g_MaxKills+val,1,40);
  Message("<c cc88ee>$showF$",0,g_MaxKills);
 
  SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL), Format("%d{{SWOR}}", g_MaxKills), ScoreboardCol(KILL));
}
