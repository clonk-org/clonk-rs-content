/*-- Tutorial --*/

#strict 2

static const g_next_mission = "MissionsHarkon.c4f\\Mission2F.c4s";
static const g_mission_access = "Mission2F";

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

//	CreateObject(_FBO, 480, 817, -1);

  CallOnRestore();
	
  AI_SetDifficulty(1);

  AssignWScriptText(LOCK, [ROCK, ["Hier geht es zum Kerker. Betreten für Unbefugte verboten."]]);
  AssignWScriptText(BED1, [BED1, ["Schlafraum. Absolute Nachtruhe um 22.00! Kein Alkohol auf den Zimmern."]]);
  AssignWScriptText(KING, [CRWN, ["Links Königliches Gemach. Rechts Thronsaal. Bei Eintritt dem Graf huldigen."]]);

	Object(1930)->MakeFenringMage();
	
  return;
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
	DoWealth(player, 10);
	
	var pClonk = GetCursor(player);
	pClonk->SetPosition(50, 589);
	pClonk->SetDir(1);
	pClonk->SetPlrViewRange(150);

	CreateContents(BOTP, pClonk);
	CreateContents(HOOK, pClonk);
	CreateContents(KNFP, pClonk);
	CreateContents(CRBW, pClonk);
//	pClonk->Hide(1, 1, 1);

	// Auftrag
//	GoalMessageJuncus(pClonk, "Deine erste Trainingsmission. Du lernst hier das Schleichen und den Umgang mit der Keule. Versuche alle Clonks in unserem Trainingslager mit der Keule zu betäuben (Doppelgraben hinter dem Opfer).", "Dabei darfst du allerdings nicht gesehen werden! Kehre dann zum Wegweiser zurück.");
  StopTime(1);
  PlayAccessibleHuts();
  return 1;
}

func Nothing() { return; }

public func MsgSignpost1()  { return "Weg von der Burg."; }

public func IsFullfilled() { return !(Object(1930)->GetAlive()); }
public func FullfillText() { return "Maron ist noch am Leben!"; }

public func GoalText()
{
	if(g_StoryIndex == 0)
		return "Töte Maron und kehre zurück zum Wegweiser.";
}
