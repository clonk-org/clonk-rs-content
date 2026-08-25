/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

local pMage;
local pClonk;
local pMalvin;
local pFred;

local pGuard1;
local pGuard2;

func Start()
{
  DoPlayMusic("The Pyre.ogg");  
	ScheduleCall(this, "DoStart", 1, 1);
}

func DoStart()
{
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Rockfort", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	pClonk->SetPosition(35, 589);

	StartFilm();
	
	pMage = CreateObject(MAGE, 380, 630+10, -1);
	pMage->MakeJuncus();
	
	pMalvin = CreateObject(ASAS, 299, 800+10, -1);
	pMalvin->MakeMalvin();
	
	pFred = CreateObject(ASAS, 406, 799+10, -1);
	pFred->MakeFred();

	SetDirTo(pFred, pMalvin);
	SetDirTo(pMalvin, pFred);
	
	Object(1282)->SetSolidMask();

	PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(200);
	FilmFadeDark();

	DialogMessage([BO69, "Buch Harkon, Kapitel 2", "1"], "Harkon hat zwar das Buch gefunden, aber da Marc am Treffpunkt nicht aufgetaucht ist, muss er nun alleine zurück nach Rockfort gehen.", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	pClonk->SetCommand(pClonk, "MoveTo", 0, 228, 629);
	FilmFadeIn();
	SetNext("Text1", 1);
}

func Text1()
{
  // Auftrag
  DialogMessage(pClonk, "Macht mir auf, ich komme von meiner Mission zurück!", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	Object(880)->ControlDown();
	pMage->SetCommand(pMage, "MoveTo", 0, 285, 630);
	pMage->AddCommand(pMage, "Wait", 0, 0, 0, 0, 0, 25);
	pMage->AddCommand(pMage, "MoveTo", 0, 315, 630);
	DialogMessage(pMage, "Hast du das rote Buch? Warum kommst du alleine?", "Text3", 0, 0, "Dialog3.ogg");
	PosCam(pMage);
}

func Text3()
{
  // Auftrag
  DialogMessage(pClonk, "Das Buch habe ich zwar, aber Marc ist nicht beim Treffpunkt erschienen!", "Text4", 0, 0, "Dialog4.ogg");
	PosCam(pClonk);
	pClonk->JumpUp();
}

func Text4()
{
  // Auftrag
  DialogMessage(pMage, "Hmm, das ist sehr beunruhigend. Er muss gefangen genommen worden sein...", "Text5", 0, 0, "Dialog5.ogg");
	PosCam(pMage);
}

func Text5()
{
  // Auftrag
  DialogMessage(pMage, "Gehen wir los, ich kenne einen geheimen Eingang zu Fenrings Kerkerkomplex.", "Text6", 0, 0, "Dialog6.ogg");
}

func Text6()
{
	pClonk->SetCommand(pClonk, "MoveTo", 0, 18, 588);
	pMage->SetCommand(pMage, "MoveTo", 0, 18, 588);
	PosCam(pFred);
	SetNext("Text7", 1);
	pFred->SetCommand(pFred, "MoveTo", 0, 335, 797);
	Object(1280)->DoOpen(pFred);
}

func Text7()
{
  // Auftrag
  DialogMessage(pFred, "Hey, hast du schon gehört, dass Harkon versagt hat bei seiner Mission?", "Text8", 0, 0, "Dialog7.ogg");
}

func Text8()
{
  // Auftrag
  DialogMessage(pMalvin, "Nein, Marc wurde bestimmt einfach in die Falle gelockt, Harkon hat sein Bestes gegeben.", "Text9", 0, 0, "Dialog8.ogg");
	PosCam(pMalvin);
}

func Text9()
{
  // Auftrag
  DialogMessage(pFred, "Ach, Juncus holt ihn da sicher wieder raus. Aber Harkon hat seine Lektion erteilt bekommen dafür dass er sich immer so vordrängt!", "Text10", 0, 0, "Dialog9.ogg");
	pFred->JumpUp();
	PosCam(pFred);
}

func Text10()
{
	FilmFadeOut();
	SetNext("EndIntro", 4);
}

func EndIntro()
{
	StopFilm();
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Map", 0);
  // Alles wiederherstellen
  RestoreSectObjs();

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	
	StartFilm();
	FilmFadeIn();

	pGuard1 = Object(2046);
	pGuard2 = Object(2044);

	pGuard1->SetPortrait("1");
	pGuard2->SetPortrait("2");
	
	pClonk->SetPosition(949, 588);

	PosCam(Object(2046), 0, 1);
  g_pCamera->SetPlrViewRange(40);
	SetNext("TextB1", 1);
}

func TextB1()
{
	DialogMessage(pGuard1, "Weißt du, warum der Aufzug nicht mehr geht? Wir kommen hier unten nicht mehr raus!", "TextB2", 0, 0, "Dialog10.ogg");
}

func TextB2()
{
	DialogMessage(pGuard2, "Hmm, wir müssten mal in den Maschinenraum schauen. Vielleicht fehlt Brennmaterial?", "TextB3", 0, 0, "Dialog11.ogg");
}

func TextB3()
{
	DialogMessage(pGuard1, "So ein Mist. Der ist nämlich abgesperrt...", "TextB4", 0, 0, "Dialog12.ogg");
}

local pCook;
local pScroll;

func TextB4()
{
	SetCommand(pClonk, "MoveTo", 0, 827, 560);
	DialogMessage(pClonk, "Hier ist also der geheime Zugang zum Kerkerkomplex. Dann werde ich mal schauen, ob ich Marc da irgendwo finde.", "EndIntro2", 0, 0, "Dialog13.ogg");
}

func ScriptCook()
{
	pCook = Object(1759);
	pCook->SetDir(0);
	pCook->JumpUp();
	StopClonkEx(pClonk);
  
	DialogMessage(pCook, "Hilfe, tu mir nichts! Ich wurde nur eingesperrt, weil ich das Essen versalzen hatte! Und ich verrate auch niemandem, dass der Schlüssel im Backofen ist!", "Nothing", 0, 0, "Dialog14.ogg");
}

local pFinder;
static g_ScrollFound;

func ScriptScroll(pClonk)
{
	pScroll = Object(2100);
	pFinder = pClonk;
	SetCommand(pFinder, "MoveTo", 0, GetX(pScroll), GetY(pScroll));
	DialogMessage(pFinder, "Eine Nachricht in Marcs leerer Zelle? Was hat das zu bedeuten?", "ScriptScroll2", 0, 0, "Dialog15.ogg");
}

func ScriptScroll2()
{
	g_ScrollFound = 1;
	g_StoryIndex = 1;
	DialogMessage(pFinder, "Oh nein! Sie haben ihn abgeholt und bringen ihn zur Hinrichtung! Ich muss schnell hier raus und das Juncus berichten. Also zurück zum Wegweiser am Anfang.", "Nothing", 0, 0, "Dialog16.ogg");
	pScroll->RemoveObject();
}

func Nothing() {}

func EndIntro2()
{
	StopFilm();
	StartSzen();
}


//------------------------ Intro Ende ----------------------

func DoStartSzen()
{
	DoPlayMusic("The fall of Gilead.ogg");
	var pCook = Object(1759);
  var iClr = pCook->GetColorDw();
  pCook->SetOwner(11);
  pCook->SetColorDw(iClr);
  pCook->SetPortrait("Cook1", pCook, _PRT);
	CallOnPlayerNear("ScriptCook", pCook, this, 40);
	var pScroll = Object(2100);
	CallOnPlayerNear("ScriptScroll", pScroll, this, 40);
}

public func SignpostReached()
{
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	DialogMessage(pClonk, "Puh ich hoffe Juncus kann mir helfen Marc jetzt noch zu befreien...", "OutEnd", "Ende", 0, "Dialog17.ogg");
	return 1;
}

func OutEnd()
{
	FinishMission();
}


local NextFunc;
local FramesLeft;

func Timer()
{
	if(!FramesLeft)
	{
		FramesLeft = -1;
		Call(NextFunc);
	}
	if(FramesLeft > 0) FramesLeft--;
}

func SetNext(next, frames) { NextFunc = next; FramesLeft = frames; }