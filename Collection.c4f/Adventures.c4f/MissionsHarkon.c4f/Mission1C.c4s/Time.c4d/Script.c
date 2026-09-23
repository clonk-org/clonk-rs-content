/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; SetPosition(0,0); }

protected func MenuQueryCancel() { return 1; }

local pKaftlak;
local pMessenger;
local pClonk;
local pMage;

func Start()
{
	DoPlayMusic("The Pyre.ogg");
	ScheduleCall(this, "DoStart", 1, 1);
}

func DoStart()
{
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Kanderia", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));

	StartFilm();

	pKaftlak = Object(19272);
	pKaftlak->SetPortrait("Leader3");
	pKaftlak->SetName("Kaftlak");

	pMessenger = Object(19133);
	pMessenger->SetName("Werimit");

	PosCam(pMessenger, 0, 1);
  g_pCamera->SetPlrViewRange(40);

	FilmFadeDark();

	DialogMessage([BO69, "$Story01$", "1"], "$Story02$", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
	SetCommand(pMessenger, "MoveTo", 0, 681, 506);
	DoPlayMusic("Tabuk.ogg");
}
// SetCommand(this, "MoveTo", 0, 536, 506)
func Text1()
{
  // Auftrag
  DialogMessage(pMessenger, "$Story03$", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	DialogMessage(pKaftlak, "$Story04$", "Text3", 0, 0, "Dialog3.ogg");
	SetCommand(pKaftlak, "MoveTo", 0, 536, 506);
}

func Text3()
{
	DialogMessage(pMessenger, "$Story05$", "Text4", 0, 0, "Dialog4.ogg");
	SetCommand(pMessenger, "MoveTo", 0, 614, 508);
}

func Text4()
{
	DialogMessage(pKaftlak, "$Story06$", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
	DialogMessage(pMessenger, "$Story07$", "Text6", 0, 0, "Dialog6.ogg");
	pMessenger->JumpUp();
}

func Text6()
{
	DialogMessage(pKaftlak, "$Story08$", "Text7", 0, 0, "Dialog7.ogg");
}

func Text7()
{
	DialogMessage(pMessenger, "$Story09$", "Text8", 0, 0, "Dialog8.ogg");
	pMessenger->JumpUp();
}

func Text8()
{
	DialogMessage(pKaftlak, "$Story10$", "End", "$Story11$", 0, "Dialog9.ogg");
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

	pMage = CreateObject(MAGE, 774, 70+10, -1);
	pMage->MakeJuncus();
	pMage->SetDir(1);

	for(var pCrow in FindObjects(Find_ID(BBRD)))
		pCrow->SetOwner(11);

	SetPosition(925, 68, pClonk);
	pClonk->SetDir(0);

	StartFilm();
	FilmFadeIn();
	PosCam(pMage, 0, 1);

	SetNext("In1", 1);
	DoPlayMusic("Colossus.ogg");
}

func In1()
{
	DialogMessage(pMage, "$Story12$", "In2", 0, 0, "Dialog10.ogg");
}

func In2()
{
	DialogMessage(pMage, "$Story13$", "In3", 0, 0, "Dialog11.ogg");
}

func In3()
{
	pMage->PoffClonk();
	pMage->RemoveObject();
	DialogMessage(pClonk, "$Story14$", "InEnd", "Start", 0, "Dialog12.ogg");
}

func InEnd()
{
	StopFilm();
	for(var pCrow in FindObjects(Find_ID(BBRD)))
		pCrow->SetOwner(-1);
	StartSzen();
}

func DoStartSzen()
{
	if(g_fIntroSkiped) DoPlayMusic("Colossus.ogg");
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	CallOnCondition("ScriptFinished", this, "ScriptFinishedCondition");
}

func ScriptFinishedCondition() { return GameCall("IsFullfilled"); }

func ScriptFinished() {
	DialogMessage(pClonk, "$Story15$", "Nothing", 0, 0, "Dialog13.ogg");
}

public func SignpostReached()
{
	DialogMessage(pClonk, "$Story16$", "Out1", 0, 0, "Dialog14.ogg");
	return 1;
}

func Out1()
{
	pMage = CreateObject(MAGE, 774, 70+10, -1);
	pMage->MakeJuncus();
	pMage->SetDir(1);
	pMage->PoffClonk();
	DoPlayMusic("Epic Unease.ogg");
	DialogMessage(pMage, "$Story17$", "Out2", 0, 0, "Dialog15.ogg");
}

func Out2()
{
	DialogMessage(pClonk, "$Story18$", "Out3", 0, 0, "Dialog16.ogg");
}

func Out3()
{
	DialogMessage(pMage, "$Story19$", "OutEnd", "$Story11$", 0, "Dialog17.ogg");
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