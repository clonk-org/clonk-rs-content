/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; SetPosition(0,0); }

protected func MenuQueryCancel() { return 1; }

local pMage;
local pClonk;

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

	StartFilm();
	
	pMage = CreateObject(MAGE, 460, 808+10, -1);
	pMage->MakeJuncus();
	pMage->CastObjects(SPRK, 10, 20);
	pMage->SetPlrViewRange(10);

	pClonk->SetPosition(511, 820);
	pClonk->SetPlrViewRange(150);
	
	SetDirTo(pMage, pClonk);
	SetDirTo(pClonk, pMage);

	// Betten füllen
	Object( 297)->CreateContents(CLNK);
	Object(1287)->CreateContents(CLNK);
	Object( 297)->SetAction("Occupied");
	Object(1287)->SetAction("Occupied");

	PosCam(pMage, 0, 1);
	FilmFadeDark();

	DialogMessage([BO69, "$Story01$", "1"], "$Story02$", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
}

func Text1()
{
  // Auftrag
  DialogMessage(pMage, "$Story03$", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	DialogMessage(pClonk, "$Story04$", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
	DialogMessage(pMage, "$Story05$", "Text3b", 0, 0, "Dialog4.ogg");
}

func Text3b()
{
	DialogMessage(pClonk, "$Story06$", "Text4", 0, 0, "Dialog5.ogg");
}

func Text4()
{
//	Object(1283)->DoOpen();
//	Object(1283)->OpenEntrance();
//	SetCommand(pMage, "MoveTo", 0, 385, 800);
//	SetCommand(pClonk, "MoveTo", 0, 410, 800);
	pMage->SetMacroCommand("MoveTo", 0, 385, 800);
	pClonk->SetMacroCommand("MoveTo", 0, 414, 800);
	SetNext("Text5", 1);
}

func Text5()
{
	DialogMessage(pMage, "$Story07$", "Text6", 0, 0, "Dialog6.ogg");
}

func Text6()
{
	pMage->SetDir(1);
	DialogMessage(pMage, "$Story08$", "Text7", 0, 0, "Dialog7.ogg");
}

func Text7()
{
	PosCam(pClonk);
	pMage->SetMacroCommand("Enter", Object(1273));
/*	AddCommand(pMage, "Enter", Object(1273));
	AddCommand(pMage, "MoveTo", 0, 461, 690);
  AddCommand(pMage, "Call", Object(1278), 0,0,0,0, "ContainedUp");
	AddCommand(pMage, "Enter", Object(1278));*/
	pClonk->SetMacroCommand("Enter", Object(1284));
	pMage->SetPlrViewRange(0);
//	SetCommand(pClonk, "MoveTo", 0, 410, 800);
	SetNext("End", 2);
}

func Nothing() { return; }

func End()
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

	StartSzen();
}

func EndIntro()
{
	StopFilm();
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Map", 0);
  // Alles wiederherstellen
  RestoreSectObjs();

	pMage = CreateObject(MAGE, 118, 238+10, -1);
	pMage->MakeJuncus();
	pMage->SetDir(0);

	SetPosition(31, 209, pClonk);
	pClonk->SetDir(1);

	StartFilm();
	FilmFadeIn();
	PosCam(pMage, 0, 1);

	SetNext("In1", 1);
	DoPlayMusic("The Forest and the Trees.ogg");
}

func In1()
{
	DialogMessage(pMage, "$Story09$", "In2", 0, 0, "Dialog8.ogg");
}

func In2()
{
	DialogMessage(pMage, "$Story10$", "In3", 0, 0, "Dialog9.ogg");
}

func In3()
{
	pMage->PoffClonk();
	pMage->RemoveObject();
	DialogMessage(pClonk, "$Story11$", "InEnd", "Start", 0, "Dialog10.ogg");
}

func InEnd()
{
	StopFilm();
	for(var pCrow in FindObjects(Find_ID(BBRD)))
		pCrow->SetOwner(-1);
	StartSzen();
}

//------------------------ Intro Ende ----------------------

func DoStartSzen()
{
	if(g_fIntroSkiped) DoPlayMusic("The Forest and the Trees.ogg");
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));

	CallOnCondition("ScriptFinished", this, "ScriptFinishedCondition");
}

func ScriptFinishedCondition() { return GameCall("IsFullfilled"); }

func ScriptFinished() {
	DialogMessage(pClonk, "$Story12$", "Nothing", 0, 0, "Dialog11.ogg");
}

// ----------------------- Outro -----------------------

public func SignpostReached()
{
	DialogMessage(pClonk, "$Story13$", "Out1", 0, 0, "Dialog12.ogg");
	return 1;
}

func Out1()
{
	pMage = CreateObject(MAGE, 321, 179+10, -1);
	pMage->MakeJuncus();
	pMage->PoffClonk();
	pMage->SetDir(0);

	pClonk->SetDir(1);
	pClonk->SetAction("Walk");
	DoPlayMusic("Epic Unease.ogg");
	DialogMessage(pMage, "$Story14$", "OutEnd", "$Story15$", 0, "Dialog13.ogg");
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