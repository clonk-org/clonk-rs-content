/*-- Tutorial --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2B.c4s";
static const g_mission_access = "Mission2B";

static const g_chapter = 2;

func DoStartSzen()
{
	for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(FLNT,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(ROCK,Material("Earth"),0);
  for(var cnt=0;cnt<15;cnt++) PlaceInMaterial(BONE,Material("Earth"),0);
  for(var cnt=0;cnt<10;cnt++) PlaceInMaterial(LOAM,Material("Earth"),0);

  CreateObject(LENS, 320, 150, -1);
	PlaceSunLight();

	SetHostility(10, 11, 1, 1, 1);
	SetHostility(11, 10, 1, 1, 1);

	CreateObject(_FBO, 480, 817, -1);
	
	Object(1796)->LocalN("fDeactivated") = 1;//SetCategory(1); // Aufzüge ausschalten
	Object(1529)->LocalN("fDeactivated") = 1;//->SetCategory(1);

	if(Object(1930)) Object(1930)->RemoveObject(); // Hofmagier
	Object(1916)->RemoveObject(); // Wache1
	Object(1899)->RemoveObject(); // Wache1
	Object(1836)->RemoveObject(); // Brandbombe

//  CallOnRestore();

  AI_SetDifficulty(1);

  AssignWScriptText(LOCK, [ROCK, ["Hier geht es zum Kerker. Betreten für Unbefugte verboten."]]);
  AssignWScriptText(BED1, [BED1, ["Schlafraum. Absolute Nachtruhe um 22.00! Kein Alkohol auf den Zimmern."]]);
  AssignWScriptText(KING, [CRWN, ["Links Königliches Gemach. Rechts Thronsaal. Bei Eintritt dem Graf huldigen."]]);
	
  for (var pObj in FindObjects(Find_Func("IsTree")))
  {
    if(GetID(pObj)==WMIL) continue;
    if(!GetEffect("NoBurning", pObj))
      AddEffect("NoBurning", pObj, 200, 0);
  }
}

global func FxNoBurningEffect(szNewEffect, iEffectTarget, iEffectNumber, iNewEffectNumber, var1, var2, var3)
  {
  // Feuer abblocken
  if (WildcardMatch(szNewEffect, "*Fire*")) return -1;
  // Alles andere ist OK
  return;
  }

global func FxNoBurningDamage()
  {
  // Kein Schaden
  return;
  }

protected func FxNoBurningEffect(string szNewEffect)
  {
  if(szNewEffect == "NoBurning") return -1; // Einer reicht
  }

func OnRestore()
{
	// Kessel mit Pferd
	Object(809)->SetObjDrawTransform(-800, 0, 1000, 0, 1000, 1000, this, 1);
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
	DoInitPlayer(player);
  return 1;
}

protected func PlayerStart(int player, int tx, int ty, object pBase, int iTeam)
{
	var pClonk = GetCursor(player);
	pClonk->SetPosition(1777, 719);
	pClonk->SetDir(0);
	pClonk->SetPlrViewRange(150);

	CreateContents(CLUB, pClonk);
//	DoWealth(player, 10);
	pClonk->Hide(1, 1, 1);

	// Der erste bekommt noch Geld
	if(GetPlayerByIndex(C4PT_User) == player) DoWealth(player, 10);
	
  PlayAccessibleHuts();
	return 1;
}

public func MsgSignpost1()  { return "Weg von der Burg."; }

public func IsFullfilled() { return GetID(FindObject2(Find_ID(_FBO))->Contained()) == BGST; }
public func FullfillText() { return "Du musst das Buch mitbringen!"; }

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
		return "Brich in die Burg ein und stiel das rote Buch. Kehre damit zum Wegweiser zurück.";
}