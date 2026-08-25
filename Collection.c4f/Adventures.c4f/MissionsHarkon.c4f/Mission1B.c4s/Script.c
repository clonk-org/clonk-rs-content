/*-- Training Schleichen --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission1C.c4s";
static const g_mission_access = "Mission1C";

static const g_chapter = 1;

func DoStartSzen()
{
	Object(808)->ControlLeft();
	DigFreeRect(280, 270, 340, 20);
	Object(808)->ControlLeft();
	
	Object(824)->ControlLeft();
	Object(827)->ControlRight();
	DigFreeRect(740, 210, 160, 30);
	Object(824)->ControlLeft();
	Object(827)->ControlRight();
  
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

  CreateObject(LENS, 320, 150, -1);
	PlaceSunLight();
	
	SetHostility(10, 11, 1, 1, 1);
	SetHostility(11, 10, 1, 1, 1);

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
	pClonk->SetPosition(42, 213);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(CLUB, pClonk);
//	pClonk->Hide(1, 1, 1);

	// Auftrag
	if(g_fIntroSkiped)
		GoalMessageJuncus(pClonk, "Deine erste Trainingsmission. Du lernst hier das Schleichen und den Umgang mit der Keule. Versuche alle Clonks in unserem Trainingslager mit der Keule zu betäuben (Doppelgraben hinter dem Opfer).", "Dabei darfst du allerdings nicht gesehen werden! Kehre dann zum Wegweiser zurück.");
  return 1;
}

func Nothing() { return; }

public func MsgSignpost1()  { return "Weg von der Burg."; }

public func IsFullfilled() { return !FindObject2(Find_Owner(10), Find_OCF(OCF_CrewMember), Find_Not(Find_Action("Dead"))); }
public func FullfillText() { return "Es sind noch Clonks im Lager bei Bewusstsein!"; }

public func GoalText() { return Format("Betäube alle Clonks im Lager ohne gesehen zu werden. Es sind noch %d übrig.", ObjectCount2(Find_Owner(10), Find_OCF(OCF_CrewMember), Find_Not(Find_Action("Dead")))); }

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
