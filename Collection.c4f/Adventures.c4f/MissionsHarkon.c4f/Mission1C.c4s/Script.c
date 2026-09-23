/*-- Training Armbrust --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission1D.c4s";
static const g_mission_access = "Mission1D";

static const g_chapter = 1;

func DoStartSzen()
{
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

	PlaceSunLight();
	for(var i = 0; i < 10; i++)
		CreateObject(BBRD, 100+RandomX(-50,50, 100), 10, 10);

  AI_SetDifficulty(1);
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
	SetPosition(925, 68, pClonk);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(BOTP, pClonk);
  CreateContents(CRBW, pClonk);
	pClonk->Hide(1, 1, 1);

  // Auftrag
	if(g_fIntroSkiped)
		GoalMessageJuncus(pClonk, "$Goal01$", "$Goal02$");
  return 1;
}

public func MsgSignpost1()  { return "$Goal03$"; }

public func IsFullfilled() { return !FindObject2(Find_ID(BBRD)); }
public func FullfillText() { return "$Goal04$"; }

public func GoalText() { return Format("$Goal05$", ObjectCount2(Find_ID(BBRD))); }