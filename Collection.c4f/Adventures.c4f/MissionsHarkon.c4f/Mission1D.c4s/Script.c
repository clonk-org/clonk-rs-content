/*-- Training Enterhaken --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission1E.c4s";
static const g_mission_access = "Mission1E";

static const g_chapter = 1;

func DoStartSzen()
{
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);
	
	PlaceSunLight();
	CreateObject(LENS, 622, 160, -1);
	
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
	pClonk->SetPosition(978, 569);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(HOOK, pClonk);
	CreateContents(CRBW, pClonk);
	pClonk->Hide(1, 1, 1);

	// Der erste bekommt noch Geld
	if(GetPlayerByIndex(C4PT_User) == player) DoWealth(player, 10);
	
	// Auftrag
	if(g_fIntroSkiped)
		GoalMessageJuncus(pClonk, "Nun kommen wir zur dritten Mission. Hier musst du deinen Enterhaken einsetzen um an den Wegweiser am anderen Ende der Schlucht zu kommen. Dabei darf dich allerdings keiner der Clonks sehen!");
  return 1;
}

func AI_EncounterENMY(pEnemy, pTarget)
{
  DisableAI();
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var pObj = GetCursor(GetPlayerByIndex(i, C4PT_User));
		if(pTarget == pObj)
			pObj->OpenRelauchMenu("Du wurdest entdeckt!");
		else
			pObj->OpenRelauchMenu(Format("%s wurdest entdeckt!", GetName(pTarget)));
	}
}

// KI ausschalten
global func DisableAI()
{
  // Alle Physicals aktualisieren
  for (var obj in FindObjects(Find_OCF(OCF_Alive), Find_Func("IsAI")))
    obj->AI_Disable();
}

public func MsgSignpost1()  { return "Ende der Schlucht."; }

public func IsFullfilled() { return 1; }
public func FullfillText() { return ""; }

public func GoalText() { return "Du musst den Wegweiser ganz oben links erreichen."; }
