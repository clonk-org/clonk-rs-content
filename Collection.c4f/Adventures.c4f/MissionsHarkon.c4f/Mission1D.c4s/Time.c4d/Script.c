/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; SetPosition(0,0); }

protected func MenuQueryCancel() { return 1; }

local pMage;
local pClonk;
local pFred;
local pMalvin;

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
	
	Object(1361)->SetPlrViewRange(0);
	Object(1360)->SetPlrViewRange(0);
	
	pFred = CreateObject(ASAS, 465, 741+10, -1);
	pFred->MakeFred();
	pFred->SetDir(0);
	pFred->SetAction("Idling");

	pMalvin = CreateObject(ASAS, 444, 742+10, -1);
	pMalvin->MakeMalvin();
	pMalvin->SetDir(1);
	pMalvin->SetAction("Idling");
	pMalvin->SetPhase(8);
//	pMalvin->SetPlrViewRange(40);

	pClonk->SetPosition(511, 820);
	
	SetDirTo(pMage, pClonk);
	SetDirTo(pClonk, pMage);

	// Clonk startet im Bett
	pClonk->Enter(Object(1284));
	Object(1284)->SetAction("Occupied");

	PosCam(Object(1284), 0, 1);
  g_pCamera->SetPlrViewRange(40);

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
	DialogMessage(pMalvin, "$Story03$", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	Object(1283)->DoOpen();
	Object(1283)->OpenEntrance();

	pClonk->Exit();
	pClonk->SetPosition(500, 819);
	
  AddCommand(pClonk, "MoveTo", 0, 424, 741);
  AddCommand(pClonk, "Call", Object(1278), 0,0,0,0, "ContainedUp");
	AddCommand(pClonk, "Enter", Object(1278));
	AddCommand(pClonk, "Exit");

	PosCam(pClonk);
	DialogMessage(pFred, "$Story04$", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
	pMalvin->SetDir(0);

	DialogMessage(pClonk, "$Story05$", "Text4", 0, 0, "Dialog4.ogg");
}

func Text4()
{
	DialogMessage(pMalvin, "$Story06$", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
	DialogMessage(pFred, "$Story07$", "Text6", 0, 0, "Dialog6.ogg");
}

func Text6()
{
	DialogMessage(pMalvin, "$Story08$", "Text7", 0, 0, "Dialog7.ogg");
}

func Text7()
{
	DialogMessage(pClonk, "$Story09$", "Text8", 0, 0, "Dialog8.ogg");
}

func Text8()
{
	SetDirToPermanent(pClonk, pFred);
	SetDirToPermanent(pClonk, pMalvin);

	AddCommand(pClonk, "MoveTo", Object(874));
  AddCommand(pClonk, "Call", Object(1273), 0,0,0,0, "ContainedUp");
	AddCommand(pClonk, "Enter", Object(1273));
	AddCommand(pClonk, "Jump", 0, 457, 690);
	AddCommand(pClonk, "MoveTo", 0, 516, 709);

	DialogMessage(pFred, "$Story10$", "End", "$Story11$", 0, "Dialog9.ogg");
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
	pMage = CreateObject(MAGE, 884, 559+10, -1);
	pMage->MakeJuncus();
	pMage->SetDir(1);

	SetPosition(978, 569, pClonk);
	pClonk->SetDir(0);
	pClonk->StopClonkEx();
	
	StartFilm();
	FilmFadeIn();
	DoPlayMusic("Colossus.ogg");
	PosCam(pMage, 0, 1);

	SetNext("In1", 1);
}

func In1()
{
	DialogMessage(pMage, "$Story12$", "In2", 0, 0, "Dialog10.ogg");
}

func In2()
{
	DialogMessage(pClonk, "$Story13$", "In3", 0, 0, "Dialog11.ogg");
}

func In3()
{
	DialogMessage(pMage, "$Story14$", "In4", 0, 0, "Dialog12.ogg");
}

func In4()
{
	DoWealth(GetOwner(pClonk), 5);
	pClonk->Sound("Cash");
	DialogMessage(pMage, "$Story15$", "In5", 0, 0, "Dialog13.ogg");
}

func In5()
{
	pMage->PoffClonk();
	pMage->RemoveObject();
	DialogMessage(pClonk, "$Story16$", "InEnd", "Start", 0, "Dialog14.ogg");
}

func InEnd()
{
	StopFilm();
	DoWealth(GetOwner(pClonk), -5);
	StartSzen();
}

func DoStartSzen()
{
	if(g_fIntroSkiped) DoPlayMusic("Colossus.ogg");
}

public func SignpostReached()
{
	pClonk = GetCrew(0, GetPlayerByIndex(0, C4PT_User));
	DialogMessage(pClonk, "$Story17$", "Out1", 0, 0, "Dialog15.ogg");
	return 1;
}

func Out1()
{
	CreateObject(FBRG, 207, 102, -1);
	pMage = CreateObject(MAGE, 207, 87+10, -1);
	pMage->MakeJuncus();
	pMage->SetDir();
	pMage->PoffClonk();
	DoPlayMusic("Epic Unease.ogg");
	DialogMessage(pMage, "$Story18$", "Out2", 0, 0, "Dialog16.ogg");
}

func Out2()
{
	DialogMessage(pClonk, "$Story19$", "Out3", 0, 0, "Dialog17.ogg");
}

func Out3()
{
	DialogMessage(pMage, "$Story20$", "OutEnd", "$Story11$", 0, "Dialog18.ogg");
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