/*-- Fenrings Versteck --*/

#strict 2

static const g_next_mission = 0;//"MissionsHarkon.c4f\\Mission3A.c4s";
static const g_mission_access = "Mission3A";

static const g_chapter = 2;

func DoStartSzen()
{
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

  PlaceSunLight();
  AssignWScriptText(SKUL, [SKUL, ["Kerkerraum des Todes mit gaaanz vielen tödlichen und bösen Stacheln."]]);

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
  SetPosition(76, 88, pClonk);
  pClonk->SetDir(1);
  pClonk->SetPlrViewRange(150);

  CreateContents(BOTP, pClonk);
  CreateContents(CRBW, pClonk);
  pClonk->Hide(1, 1, 1);

  // Auftrag
//  if(g_fIntroSkiped)
//	  GoalMessageJuncus(pClonk, "Finde Fenring und besiege ihn.");
  for(var pObj in FindObjects(Find_ID(CAVE)))
    pObj->SetCategory(1);
  PlayAccessibleHuts();
  return 1;
}

public func MsgSignpost1()  { return "Höhle verlassen"; }

public func IsFullfilled() { return false; }
public func FullfillText() { return "Fenring lebt noch"; }

public func GoalText() { return "Fenring muss noch besiegt werden"; }