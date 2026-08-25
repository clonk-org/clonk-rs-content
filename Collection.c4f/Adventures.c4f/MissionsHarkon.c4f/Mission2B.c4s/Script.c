/*-- Tutorial --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2C.c4s";
static const g_mission_access = "Mission2C";

static const g_chapter = 2;

func DoStartSzen()
{
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

	SetHostility(10, 11, 1, 1, 1);
	SetHostility(11, 10, 1, 1, 1);

  CallOnRestore();
	
  AI_SetDifficulty(1);

  AssignWScriptText(COAL, [POWR, ["Maschienenraum, betreten nur mit Gräflicher Genehmigung. Wer was kaputt macht wird verprügelt!"]]);
  AssignWScriptText(CRYS, [CRYS, ["Hochsicherheitszelle von Fukat! Keine Kristalle in der Nähe liegen lassen! Das Essen unter dem Tor durchschieben!"]]);
	
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
	pClonk->SetPosition(827, 560);
	pClonk->SetDir(0);
	pClonk->SetPlrViewRange(150);

	CreateContents(KNFP, pClonk);
	pClonk->Hide();

	// Der erste bekommt noch Geld
	if(GetPlayerByIndex(C4PT_User) == player) DoWealth(player, 10);

	return 1;
}

public func MsgSignpost1()  { return "Raus aus dem Kerker."; }

public func IsFullfilled() { return g_ScrollFound; }
public func FullfillText() { return "Du musst noch Marc befreien!"; }

public func GoalText()
{
	if(g_StoryIndex == 0)
		return "Finde Marcs Kerkerzelle und befreie Marc.";
	if(g_StoryIndex == 1)
		return "Kehre zurück zum Wegweiser um Juncus von Marcs Hinrichtung zu berichten.";
}
