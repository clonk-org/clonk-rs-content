/*-- Tutorial --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2E.c4s";
static const g_mission_access = "Mission2E";

static const g_chapter = 2;

func DoStartSzen()
{
/*  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);*/

	SetSkyAdjust(HSL(100, 128, 80));
/*  CreateObject(LENS, 320, 150, -1);
	PlaceSunLight();*/
	
	SetHostility(10, 11, 1, 1, 1);
	SetHostility(11, 10, 1, 1, 1);

  CallOnRestore();
	
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
	DoWealth(player, 10);
	
	var pClonk = GetCursor(player);
	pClonk->SetPosition(32, 503);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(CLUB, pClonk);
//	CreateContents(HOOK, pClonk);
	CreateContents(KNFP, pClonk);
//	CreateContents(CRBW, pClonk);

	if(GetPlayerByIndex(0, C4PT_User) == player && !g_pMarc)
	{
		g_pMarc = CreateObject(ASAS, 32, 503, 11);
		g_pMarc->Kill();
		g_pMarc->Message("", g_pMarc);
		g_pMarc->MakeMarc();
		pClonk->PickUpCorps(g_pMarc);
	}
	g_pBlimp = FindObject2(Find_ID(BLMP));
//	pClonk->Hide(1, 1, 1);

	// Auftrag
//	GoalMessageJuncus(pClonk, "Deine erste Trainingsmission. Du lernst hier das Schleichen und den Umgang mit der Keule. Versuche alle Clonks in unserem Trainingslager mit der Keule zu betäuben (Doppelgraben hinter dem Opfer).", "Dabei darfst du allerdings nicht gesehen werden! Kehre dann zum Wegweiser zurück.");
  
  PlayAccessibleHuts();
  return 1;
}

static g_pMarc;
static g_pBlimp;

func Nothing() { return; }

public func MsgSignpost1()  { return "Mit dem Luftschiff fliehen."; }

public func IsFullfilled() { return ( ObjectDistance(FindObject2(Find_ID(BLMP)), FindObject2(Find_ID(ASAS), Find_Owner(11))) < 50 ); }
public func FullfillText() { return "Marc muss bei dir sein!"; }

func AI_EncounterENMY()
{
  DisableAI();
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		GetCursor(GetPlayerByIndex(i, C4PT_User))->OpenRelauchMenu("Du wurdest entdeckt!");
}

// KI ausschalten
global func DisableAI()
{
  // Alle Physicals aktualisieren
  for (var obj in FindObjects(Find_OCF(OCF_Alive), Find_Func("IsAI")))
    obj->AI_Disable();
}

public func GoalText()
{
	if(g_StoryIndex == 0)
		return "Bringe den bewustlosen Marc zum Luftschiff.";
}