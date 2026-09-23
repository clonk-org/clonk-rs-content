/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

local pMage;
local pMarc;
local pBlimp;
local pClonk;
local pGuard;

func Start(pNewClonk)
{
  DoPlayMusic("The Pyre.ogg");
	pClonk = pNewClonk;
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
	
	pMage = CreateObject(MAGE, 677, 221+10, -1);
	pMage->MakeJuncus();
	pMage->SetDir(0);
	
	pBlimp = CreateObject(BLMP, 0, 0, -1);
	pBlimp->SetPosition(140, -13);
	pBlimp->SetDir(1);
	
	pClonk->SetPosition(5+pBlimp->GetX(), 17+pBlimp->GetY());
	pClonk->SetAction("Push", pBlimp);
	pClonk->SetDir(1);
	pMarc = CreateObject(ASAS, 0, 0, -1);
	pMarc->MakeMarc();
	pMarc->SetPosition(-6+pBlimp->GetX(), 17+pBlimp->GetY());
	pMarc->SetDir(1);
	
	pGuard = FindObject(KNIG, 494, 250, -1, -1);
	pGuard->SetOwner(-1);
	
	Object(293)->SetCategory(1);
	
	PosCam(pBlimp, 0, 1);
  g_pCamera->SetPlrViewRange(200);
	FilmFadeDark();

	DialogMessage([BO69, "$Story01$", "1"], "$Story02$", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 8);
	pBlimp->SetPosition(140, -13);
	pBlimp->SetDir(1);
	pClonk->SetPosition(5+pBlimp->GetX(), 17+pBlimp->GetY());
	pMarc->SetPosition(-6+pBlimp->GetX(), 17+pBlimp->GetY());
	
	SetCommand(pBlimp, "Call", this, 0, 0, 0, "Text2"); 
	AddCommand(pBlimp, "MoveTo", 0, 530, 234);
}

func Text1()
{
  // Auftrag
  DialogMessage(pGuard, "$Story03$", "Text2", 0, 0, "Dialog2.ogg");
	PosCam(pBlimp);
}

local fBlimpLanded;

func Text2()
{
	if(fBlimpLanded == 0)
	{
		fBlimpLanded = 1;
		return;
	}
	AddCommand(pClonk, "MoveTo", 0, 638, 250);
	AddCommand(pMarc,  "MoveTo", 0, 582, 250);
	DialogMessage(pMage, "$Story04$", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
  DialogMessage(pClonk, "$Story05$", "Text4", 0, 0, "Dialog4.ogg");
}

func Text4()
{
  DialogMessage(pMage, "$Story06$", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
  DialogMessage(pMarc, "$Story07$", "Text6", 0, 0, "Dialog6.ogg");
	pClonk->SetDir(0);
}

func Text6()
{
  DialogMessage(pMage, "$Story08$", "Text7", 0, 0, "Dialog7.ogg");
	pClonk->SetDir(1);
}

func Text7()
{
  DialogMessage(pMage, "$Story09$", "Text8", 0, 0, "Dialog8.ogg");
}

func Text8()
{
  DialogMessage(pClonk, "$Story10$", "Text9", 0, 0, "Dialog9.ogg");
}

func Text9()
{
  DialogMessage(pMage, "$Story11$", "Text10", 0, 0, "Dialog10.ogg");
}

func Text10()
{
	PosCam(pMarc);
	pMage->PoffClonk();
	pMage->SetPosition();
	SetNext("Text11", 1);
}

func Text11()
{
  DialogMessage(pMarc, "$Story12$", "Text12", 0, 0, "Dialog11.ogg");
	pClonk->SetDir(0);
}

func Text12()
{
  DialogMessage(pClonk, "$Story13$", "Text13", 0, 0, "Dialog12.ogg");
}

func Text13()
{
	SetDirToPermanent(pClonk, pMarc);
	SetCommand(pClonk, "Call", this, 0, 0, 0, "Text14"); 
	AddCommand(pClonk, "MoveTo", 0, 5+pBlimp->GetX(), 17+pBlimp->GetY());
}

func Text14()
{
	pClonk->SetAction("Push", pBlimp);
	SetCommand(pBlimp, "MoveTo", 0, 140, -13);
	SetNext("DoStart2", 8);
	FilmFadeOut();
}

func DoStart2()
{
	StopFilm();
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Map", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	pClonk->SetPosition(50, 589);
	pClonk->SetDir(1);
	
	StartFilm();

	PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(40);

	FilmFadeIn();
	SetNext("TextB1", 1);
}

func TextB1()
{
  // Auftrag
  DialogMessage(pClonk, "$Story14$", "EndIntro", 0, 0, "Dialog13.ogg");
}

func EndIntro()
{
	StopFilm();
	StartSzen();
}

func DoStartSzen()
{
	DoPlayMusic(["Heart of Medieval.ogg", "Pactum serva.ogg"]);

  CallOnCondition("ScriptFinished", this, "ScriptFinishedCondition");
}

func ScriptFinishedCondition() { return GameCall("IsFullfilled"); }

func ScriptFinished() {
  DialogMessage(pClonk, "$Story15$", "Nothing", 0, 0, "Dialog14.ogg");
}

func Nothing() {}

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