/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

func Start()
{
	ScheduleCall(this, "DoStart", 1, 1);
}

local pClonk;
local pKaftlak;
local pWerimitn;

func DoStart()
{
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Kanderia", 3);
  // Alles wiederherstellen
  RestoreSectObjs();
	
	DoPlayMusic("The Pyre.ogg");

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));

	StartFilm();

	pKaftlak = FindObject(KAND, 1026, 227, -1, -1); 
	pKaftlak->SetPortrait("Leader3");
	pKaftlak->SetName("Kaftlak");
	pKaftlak->SetDir(0);
	pKaftlak->CreateContents(SPER);
	pKaftlak->SetAction("Walk");
	
	pWerimitn = FindObject(KAND, 1200, 227, -1, -1); 


	PosCam(pKaftlak, 0, 1);
  g_pCamera->SetPlrViewRange(200);
	FilmFadeDark();

	DialogMessage([BO69, "Buch Harkon, Kapitel 2", "1"], "Harkon hat nun Marc erstmal vor dem Strick bewahrt, doch er ist noch immer nicht in Sicherheit. In der Zwischenzeit ist aber auch in Kanderien die Zeit nicht stehen geblieben.", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
	DoPlayMusic("Tabuk.ogg");
}

func Text1()
{
  // Auftrag
  DialogMessage(pKaftlak, "Die Schlachten laufen zur Zeit sehr gut, doch ich mache mir immer noch Sorgen wegen des Jungen...", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
  // Auftrag
  DialogMessage(pWerimitn, "Herr, ich habe unseren besten Späher ausgeschickt, alles über den Jungen heraus zu finden.", "Text3", 0, 0, "Dialog3.ogg");
	SetCommand(pWerimitn, "MoveTo", 0, 1072, 223);
}

func Text3()
{
  // Auftrag
  DialogMessage(pKaftlak, "Ich hoffe nur, dass wir noch nicht zu spät dran sind. Wenn jemand das Geheimnis herausfindet, dann kann das verheerende Folgen haben!", "Text4", 0, 0, "Dialog4.ogg");
	pKaftlak->SetDir(1);
}

func Text4()
{
  // Auftrag
  DialogMessage(pKaftlak, "Nun geh und berichte mir, sobald du Neuigkeiten über die Sache hast. Und wie gesagt, strengste Geheimhaltung!", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
	SetCommand(pKaftlak, "MoveTo", 0, 1154, 249);
	SetCommand(pWerimitn, "MoveTo", 0, 1251, 245);

	FilmFadeOut();
	SetNext("DoStart2", 4);
}

func DoStart2()
{
	StopFilm();
	// Starte in einer anderen Section
  pClonk = GetHiRank(GetPlayerByIndex(0, C4PT_User));
  pClonk->Hide(1, 1, 1);
  SaveObjects();
	LoadScenarioSection("Map", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	SetSkyAdjust(HSL(100, 128, 80));
	
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	pClonk->SetPosition(32, 503);
	pClonk->SetDir(1);
	
	g_pMarc = CreateObject(ASAS, 32, 503, 11);
	g_pMarc->Kill();
	g_pMarc->Message("", g_pMarc);
	g_pMarc->MakeMarc();
	pClonk->PickUpCorps(g_pMarc);

	g_pBlimp = FindObject2(Find_ID(BLMP));
  LocalN("fUnlocked", g_pBlimp) = 1;
	
	StartFilm();
	FilmFadeIn();
	
	PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(40);
	SetNext("TextB1", 1);
	DoPlayMusic(["The Silver Tree.ogg", "Moon waltz.ogg"]);
}

func TextB1()
{
	DialogMessage(pClonk, "Jetzt habe ich Marc bis in den Wald geschleppt. Am anderen Ende des Waldes steht mein Luftschiff zur Flucht.", "TextB2", 0, 0, "Dialog6.ogg");
}

func TextB2()
{
	DialogMessage(pClonk, "Ich sollte mich beeilen, denn sie werden mich bestimmt verfolgen, um Marc zurück zu bekommen. Eventuell brauche ich auch noch ein Katapult, um Marc den Berg zum Luftschiff hoch zu bekommen. [Katapult anfassen, wärend Marc getragen wird]", "EndIntro", 0, 0, "Dialog7.ogg");
}

func EndIntro()
{
	StopFilm();
	StartSzen();
}

func DoStartSzen()
{
	if(g_fIntroSkiped) DoPlayMusic(["The Silver Tree.ogg", "Moon waltz.ogg"]);
	for(var pObj in FindObjects(Find_ID(_RBR)))
	{
		var color = pObj->GetColorDw();
		pObj->SetOwner(12);
		pObj->SetColorDw(color);
		pObj->SetAI();
		pObj->CreateContents(BOW1);
		pObj->CreateContents(ARWP);
		pObj->CreateContents(ARWP);
    pObj->AI_Enable(150);
	}
	SetNext("Script1", 70+20);
}

local pGuard1, pGuard2, pGuard3;

func Script1()
{
	DoPlayMusic("Breaking the siege.ogg");	

	SetPosition();
	StartFilm();
	pGuard1 = CreateObject(KNIG, 14, 495+10, 10);
	pGuard2 = CreateObject(KNIG, 52, 509+10, 10);
	pGuard3 = CreateObject(KNIG, 91, 523+10, 10);
	var aGuards = [pGuard1, pGuard2, pGuard3];
	pGuard1->SetPhysical("Walk", 50000, PHYS_Temporary);
	pGuard2->SetPhysical("Walk", 50000, PHYS_Temporary);
	pGuard3->SetPhysical("Walk", 50000, PHYS_Temporary);

	PosCam(pGuard3, 0, 1);

	for(var pGuard in aGuards)
	{
		pGuard->AttachLight();
		pGuard->SetColorDw(HSL(0+Random(50), 228, 50+Random(128)));

		pGuard->CreateContents(BOW1);
		pGuard->CreateContents(ARWP);
		pGuard->CreateContents(ARWP);
		pGuard->SetDir(1);
	}

	pGuard3->SetDir(0);
	DialogMessage(pGuard3, "Er ist hier irgendwo in den Wald gerannt mit der Leiche. Wir müssen die Leiche unbedingt finden, sonst wird Fenring bestimmt sehr wütend auf uns. Und schaut euch gut um, dass ihr auch einen getarnten Assassinen findet, da muss noch einer da sein!", "Script2", 0, 0, "Dialog8.ogg");
}

func Script2()
{
	StopFilm();

	var aGuards = [pGuard1, pGuard2, pGuard3];

	for(var pGuard in aGuards)
	{
		pGuard->SetAI();

		pGuard->LocalN("ai_fFindAssassins") = 1;
		pGuard->SetDir(1);
	}
	
	pGuard1->AI_AddPatrolPoint( 622, 521);
	pGuard1->AI_AddPatrolPoint( 693, 518);
	pGuard1->AI_AddPatrolPoint( 659, 517);
	
	pGuard2->AI_AddPatrolPoint( 997, 296);
	
	pGuard3->AI_AddPatrolPoint(1084, 529);
	pGuard3->AI_AddPatrolPoint( 980, 540);

	SetNext("Script3", 10);
}

func Script3()
{
	pGuard1 = CreateObject(KNIG, 14, 495+10, 10);
	pGuard2 = CreateObject(KNIG, 52, 509+10, 10);
	pGuard3 = CreateObject(KNIG, 91, 523+10, 10);
	
	var aGuards = [pGuard1, pGuard2, pGuard3];

	for(var pGuard in aGuards)
	{
		pGuard->AttachLight();
		pGuard->SetColorDw(HSL(0+Random(50), 228, 50+Random(128)));

		pGuard->CreateContents(BOW1);
		pGuard->CreateContents(ARWP);
		pGuard->CreateContents(ARWP);
		if(pGuard == pGuard2) pGuard->SetDir(1);
		else pGuard->SetDir(0);
		pGuard->SetAI();
	}
	
	pGuard1->AI_AddPatrolPoint(1261, 268);
	pGuard2->AI_AddPatrolPoint(1196, 220);
	pGuard3->AI_AddPatrolPoint(1114, 191);
}


// ----------------------- Outro -----------------------

local pBlimp;
local pClonk;

public func SignpostReached()
{
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	pBlimp = FindObject2(Find_ID(BLMP));
	DisableAI();
	StartFilm();
	pClonk->SetAction("Push", pBlimp);
  pBlimp->DigFreeRect(pBlimp->GetX()-34,pBlimp->GetY()-30,68,60);
  SetCommand(pBlimp, "MoveTo", 0, 981, 30);
	DialogMessage(pClonk, "Args, das war Rettung in letzter Sekunde!", "Out1", 0, 0, "Dialog9.ogg");
	PosCam(pClonk->GetX(), pClonk->GetY(), 1);
	return 1;
}

func Out1()
{
	FilmFadeOut();
	SetNext("OutEnd", 4);
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