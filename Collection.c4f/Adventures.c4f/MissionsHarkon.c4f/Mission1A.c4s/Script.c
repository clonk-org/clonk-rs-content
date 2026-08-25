/*-- Prolog --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission1B.c4s";
static const g_mission_access = "Mission1B";

static const g_chapter = 1;

func DoStartSzen()
{
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

  CreateObject(LENS, 80, 80, -1);
	
  AI_SetDifficulty(1);
	g_EA69_fAssassinModeOff = 1;
	
  PlaceSunLight();
  return;
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
	DoInitPlayer(player);
  return 1;
}

protected func PlayerStart(int player, int tx, int ty, object pBase, int iTeam)
{
	var pClonk = GetCursor(player);
	pClonk->SetPosition(1247, 316);
	pClonk->SetDir(0);
	pClonk->SetPlrViewRange(150);
  return 1;
}

public func GoalText()
{
	if(g_StoryIndex == 0)
		return "Geh nach Westen und hacke im kleinen Wald Holz.";
	if(g_StoryIndex == 1)
		return "Gehe zurück zur Hütte deines Vaters um nachzusehen, was los ist.";
	if(g_StoryIndex == 2)
		return "Besiege alle Räuber.";
	if(g_StoryIndex == -1)
		return "Ende.";
}