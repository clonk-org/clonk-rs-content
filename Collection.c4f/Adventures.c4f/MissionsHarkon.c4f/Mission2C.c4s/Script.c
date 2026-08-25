/*-- Tutorial --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2D.c4s";
static const g_mission_access = "Mission2D";

static const g_chapter = 2;

func DoStartSzen()
{
//  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
//  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

	PlaceSunLight();
	
//	SetHostility(10, 11, 1, 1, 1);
//	SetHostility(11, 10, 1, 1, 1);

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
	pClonk->SetPosition(11, 329);
	pClonk->SetDir(0);
	pClonk->SetPlrViewRange(150);

	CreateContents(KNFP, pClonk);
	pClonk->Hide(1, 1, 1);

	// Der erste bekommt noch Geld
	if(GetPlayerByIndex(C4PT_User) == player) DoWealth(player, 10);
  
  if(g_fIntroSkiped)
    GoalMessageJuncus(pClonk, "Juncus hat mir diesen Rettungstrank gegeben, mit dem man angeblich das Erhängen überleben soll.|Vielleicht sollte ich den Henker überwältigen und mich mit seinen Sachen als er ausgeben. Dann wäre ich bei der Hinrichtung als nähster bei Marc...");

  for(var pObj in FindObjects(Find_Or(Find_ID(CAVE),Find_ID(B_1D))))
    pObj->SetCategory(1);
  if(Object(2768)) Object(2768)->SetVertexXY(0,44,-78);
  if(Object(2769)) Object(2769)->SetVertexXY(0,44,-78);
  PlayAccessibleHuts();
	return 1;
}

global func ConvertCoordinates(iX, iY)
{
  var pContainer = FindObject2(Find_Func("IsInside"), Find_AtPoint(iX-GetX(), iY-GetY()));
  if(pContainer)
  {
    return Format("ObjectNumber(%d)->GetX()+%d, ObjectNumber(%d)->GetY()+%d", ObjectNumber(pContainer), -pContainer->GetX(), ObjectNumber(pContainer), iY-pContainer->GetY());
  }
  else
    return Format("%d, %d", iX, iY);
}

global func LogPatrolPoints()
{
  for(var pObj in FindObjects(Find_Func("IsAI")))
  {
    var szString = ConvertCoordinates(pObj->LocalN("ai_iHomeX"), pObj->LocalN("ai_iHomeY"));
    Log("Global(0) = [%s]; Object(%d)->LocalN(\"ai_iHomeX\") = Global(0)[0]; Object(%d)->LocalN(\"ai_iHomeY\") = Global(0)[1];", szString, ObjectNumber(pObj), ObjectNumber(pObj));
    var ai_aPatrolPoints = pObj->LocalN("ai_aPatrolPoints");
    if(ai_aPatrolPoints)
    for(var i = 0; i < GetLength(ai_aPatrolPoints); i++)
    {
      if(GetType(ai_aPatrolPoints[i]) != C4V_C4Object)
      {
        szString = ConvertCoordinates(ai_aPatrolPoints[i][0], ai_aPatrolPoints[i][1]);
        Log("Global(0) = [%s]; Object(%d)->LocalN(\"ai_aPatrolPoints\")[i][0] = Global(0)[0]; Object(%d)->LocalN(\"ai_aPatrolPoints\")[i][1] = Global(0)[1];", szString, ObjectNumber(pObj), i, ObjectNumber(pObj), i);
      }
    }
    
  }
}

public func MsgSignpost1()  { return "Weg vom Dorf."; }

public func IsFullfilled() { return 0; }
public func FullfillText() { return "Marc muss noch gerettet werden!"; }

func AI_EncounterENMY(pEncounter, pEnemy, fDead)
{
	DisableAI();
	FindObject(_STY)->Encounter(pEncounter, fDead);
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
		return "Rette Marc vor dem Galgen. Überwältige dazu den Henker und zieh dir seine Haube an.";
}