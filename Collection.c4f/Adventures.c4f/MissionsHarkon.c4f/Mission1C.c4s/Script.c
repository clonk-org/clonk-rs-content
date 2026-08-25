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
		GoalMessageJuncus(pClonk, "Deine zweite Trainingsmission beschäftigt sich mit der Armbrust. Bei der Armbrust ist es immer sehr wichtig, dass man mit dem Bolzen sparsam ist, denn sie sind oft nur sehr begrenzt vorhanden und können nach einem Treffer nicht wieder verwendet werden.", "Zur Übung musst du nun hier die schwarzen Todeskrähen abschießen. Die normalen Vögel sind jedoch nicht dein Ziel!");
  return 1;
}

public func MsgSignpost1()  { return "Canyon verlassen."; }

public func IsFullfilled() { return !FindObject2(Find_ID(BBRD)); }
public func FullfillText() { return "Es leben noch ein paar Todeskrähen!"; }

public func GoalText() { return Format("Erschieße alle schwarzen Todeskrähen. %d fehlen noch.", ObjectCount2(Find_ID(BBRD))); }