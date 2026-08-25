/*-- Fenrings Aussenposten --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2A.c4s";
static const g_mission_access = "Mission2A";

static const g_chapter = 1;

func DoStartSzen()
{
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
//  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

	PlaceSunLight();
  CreateObject(LENS, 80, 80, -1);
	
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
	pClonk->SetPosition(20, 380);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(KNFP, pClonk);
	pClonk->Hide(1, 1, 1);

  // Auftrag
	if(g_fIntroSkiped)
		GoalMessageJuncus(pClonk, "So junger Meuchelmörder. Die ist dein erster richtiger Auftrag.|Ziel ist ein abgelegner Außenposten von Graf Fenring, wo einige Goldvorräte gelagert werden. Stehle alles Gold und komme dann lebend zum Wegweiser zurück.");
	return 1;
}

public func MsgSignpost1()  { return "Weg von der Burg."; }

public func IsFullfilled() { return !FindObject2(Find_ID(GOLD)); }
public func FullfillText() { return "Du hast noch nicht alles Gold gestohlen!"; }

public func GoalText() { return Format("Stehle alles Gold aus dem Außenposten. %d Goldstücke fehlen noch.", ObjectCount2(Find_ID(GOLD))); }