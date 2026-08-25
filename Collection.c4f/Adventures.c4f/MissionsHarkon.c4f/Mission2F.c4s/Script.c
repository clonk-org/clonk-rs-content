/*-- Tutorial --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2G.c4s";
static const g_mission_access = "Mission2G";

static const g_chapter = 2;

func DoStartSzen()
{
/*  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);*/

	SetSkyAdjust(RGB(200,255,200));
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
	pClonk->SetPosition(273, 457);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(BOTP, pClonk);
	CreateContents(HOOK, pClonk);
	CreateContents(KNFP, pClonk);
	CreateContents(CRBW, pClonk);
	pClonk->Hide(1, 1, 1);

	if(GetPlayerByIndex(0, C4PT_User) == player)
	{
		SetPosition(273, 457, pClonk);
		pClonk->SetPlrViewRange(150);
		pClonk->MakeHarkon();
	}
	
  PlayAccessibleHuts();

  return 1;
}

func Nothing() { return; }

public func MsgSignpost1()  { return "Die Höhle erkunden"; }

public func IsFullfilled() { return 1; }
public func FullfillText() { return ""; }

func AI_EncounterENMY()
{
  DisableAI();
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		GetCursor(GetPlayerByIndex(i, C4PT_User))->OpenRelauchMenu("Du wurdest entdeckt!");
}

func AI_HomeReached(pTarget)
{
	FindObject(_STY)->AI_HomeReached(pTarget);
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
		return "Lass den Boten das Tor öffnen und töte ihm bevor er in die Höhle geht.";
}

global func LogRobbers()
{
  for(var pObj in FindObjects(Find_ID(_RBR)))
  {
    Log("Object(%d)->PlaceRobber(%d, %d, %d, %s);", ObjectNumber(pObj), pObj->GetX(), pObj->GetY(), pObj->GetDir(), pObj->GetName());
  }
}

global func PlaceRobber(iX, iY, iDir, szName)
{
//  if(szName != GetName())
//    Log("Oho %d %s %s", ObjectNumber(), szName, GetName());
  Exit();
  SetPosition(iX, iY);
  SetDir(iDir);
  LocalN("ai_iHomeX") = iX;
  LocalN("ai_iHomeY") = iY;
  LocalN("ai_iHomeDir") = iDir;
  LocalN("ai_aPatrolPoints") = 0;
  LocalN("aListActivitys") = 0;
  LocalN("iCurrentActivity") = 0;
  this->~SetMacroCommand("None");
  SetAction("Walk");
  LocalN("aLeisureActivitys") = 0;
  LocalN("aZeiten") = 0;
  while(Contents()) Contents()->RemoveObject();
}

global func SetRobbers()
{
Object(3246)->SetAction("Idle");
Object(3125)->PlaceRobber(1906, 197, 0, "Goddl");
Object(3124)->PlaceRobber(807, 448, 0, "Potentilla");
Object(3123)->PlaceRobber(889, 528, 1, "Serpents");
Object(3122)->PlaceRobber(114, 306, 1, "Dera");
Object(3121)->PlaceRobber(1547, 38, 0, "Reptans");
Object(3120)->PlaceRobber(627, 218, 1, "Risotto");
Object(3119)->PlaceRobber(680, 519, 1, "Mergion");
Object(3118)->PlaceRobber(451, 280, 1, "Falaffl");
Object(3412)->PlaceRobber(315, 295, 1,"");
Object(3407)->PlaceRobber(416, 265, 1,"");
Object(3411)->PlaceRobber(741, 298, 1,"");
Object(3292)->CancelActivity(Object(3412));
Object(3292)->CancelActivity(Object(3407));
Object(3292)->CancelActivity(Object(3411));
}
