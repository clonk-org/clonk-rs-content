/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

func Start()
{
  DoPlayMusic("The Pyre.ogg");
	ScheduleCall(this, "DoStart", 1, 1);
}

local pClonk;
local pMage;
local pDavid;
local pGuard;

func DoStart()
{
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Fenring", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));

	StartFilm();
	
	DisableAI();
	RemoveAll(WNWT);
	
	pMage = Object(1930);
	pMage->MakeFenringMage();
	pMage->SetAction("Dead");
	
	pDavid = CreateObject(PLDN, 737, 230+10, -1);
	pDavid->MakeDavid();
	pDavid->SetDir(1);
	
	pGuard = Object(1932);
	pGuard->SetPosition(763, 230);
	pGuard->SetDir(0);
	
	PosCam(pDavid, 0, 1);
  g_pCamera->SetPlrViewRange(200);
	FilmFadeDark();

	DialogMessage([BO69, "Buch Harkon, Kapitel 2", "1"], "Just nachdem Harkon die Burg verlassen hat, kommt David an, um bei Fenring nach dem Rechten zu sehen.", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
  DoPlayMusic("The fall of Gilead.ogg"); 
}

func Text1()
{
  DialogMessage(pDavid, "Ich möchte zu Fenring, lasst mich eintreten.", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
  DialogMessage(pGuard, "Bei Sawelô, ihr seid natürlich gerne gesehen großer Anführer des Ordens. Fenring ist leider zur Zeit nicht da, aber ihr könnt mit seinem Hofmagier Maron sprechen. Er vertritt die Geschäfte wärend Fenrings Abwesenheit.", "Text3", 0, 0, "Dialog3.ogg");
	Object(1650)->ControlLeft(pGuard);
}

func Text3()
{
	PosCam(pDavid);
	SetCommand(pDavid, "Call", this, 0, 0, 0, "Text4");
	AddCommand(pDavid, "MoveTo", 0, 874, 230);
}

func Text4()
{	
  DialogMessage(pDavid, "Maron, was ist los?", "Text5", 0, 0, "Dialog4.ogg");
}

func Text5()
{	
  DialogMessage(pDavid, "Wache, kommt schnell her, Maron ist verletzt.", "Text6", 0, 0, "Dialog5.ogg");
	pDavid->SetDir(0);
}

func Text6()
{
	SetCommand(pGuard, "Call", this, 0, 0, 0, "Text7");
	AddCommand(pGuard, "MoveTo", 0, pMage->GetX(), pMage->GetY());
	SetDirToPermanent(pGuard, pDavid);
  DialogMessage(pGuard, "Oh nein, Meister Maron, was ist passiert?", "Text7", 0, 0, "Dialog6.ogg");
}

local pMove;

func Text7()
{
	if(pMove == 0)
	{
		pMove = 1;
		return;
	}
	DialogMessage(pMage, "*röchel* ... warnt Fenring, der Zauber ist gebrochen... Heiltrank... *röchel*", "Text8", 0, 0, "Dialog7.ogg");
}

func Text8()
{
	DialogMessage(pDavid, "Ich werde sofort für ein Heilwunder beten. Anscheinend haben hier die Rebellen gewütet und versucht Maron zu töten.", "Text9", 0, 0, "Dialog8.ogg");
	pDavid->SetAction("Pray");
	SetDirToPermanent(pMage, pDavid);
}

func Text9()
{
	DialogMessage(pGuard, "Ja, deswegen hat sich Fenring schon in sein Versteck zurückgezogen. Man wollte ihn auch schon hier in der Burg beklauen. Ich muss aber nun schnellstens einen Boten losschicken, um ihn zu warnen.", "Text10", 0, 0, "Dialog9.ogg");
	pGuard->SetDir(0);
}

func Text10()
{
	PosCam(pMage);
	SetCommand(pGuard, "MoveTo", 0, 726, 230);
	FilmFadeOut();
	SetNext("DoStart2", 8);
}

func DoStart2()
{
	StopFilm();
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Map", 3);
  // Alles wiederherstellen
  RestoreSectObjs();
	
	DisableAI();
  SetRobbers();
	
	pClonk->SetPosition(273, 457);


	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
//	pClonk->SetPosition(32, 503);
	pClonk->SetDir(1);
	
	StartFilm();

	PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(40);

	FilmFadeIn();
	SetNext("TextB1", 1);
}

func TextB1()
{
	DialogMessage(pClonk, "Hmm, ich muss wohl die ganze Gegend durchsuchen, ob nicht irgendwo das Versteck von Fenring ist...", "TextB2", 0, 0, "Dialog10.ogg");
}

func TextB2()
{
	pMessenger = CreateObject(KKGT, 32, 503+10, 10);
  pMessenger->SetPortrait("Armored1", 0, KNIG);
	DialogMessage(pMessenger, "Ok, am Ende dieses Waldes ist der Berg mit Fenrings Versteck. Ich hoffe nur mal, es hausen hier nicht zu viele Räuber...", "TextB3", 0, 0, "Dialog11.ogg");
}

func TextB3()
{
  // Auftrag
  DialogMessage(pClonk, "Hey, der Bote kann mich doch zu dem Versteck führen. Ich muss nur dafür sorgen, dass ihn die Räuber nicht töten.", "TextB4", 0, 0, "Dialog12.ogg");
}

func TextB4()
{
	DialogMessage(pClonk, "Wobei wenn er mir das Versteck geöffnet hat, sollte ich ihn schnell töten, bevor er hinein geht und Fenring warnt. Vielleicht gibt es ja eine Abkürzung.", "EndIntro", 0, 0, "Dialog13.ogg");
}

func EndIntro()
{
	StopFilm();
	StartSzen();
}

local pMessenger;

func DoStartSzen()
{
	DoPlayMusic(["La busqueda de Ianna.ogg", "The Last Mission.ogg"]);
	
	WaypointsVisible(0);
	SetPosition();
	EnableAI();
  SetRobbers();
	for(var pObj in FindObjects(Find_ID(_RBR)))
	{
		var color = pObj->GetColorDw();
		pObj->SetOwner(12);
		pObj->SetColorDw(color);
		pObj->SetAI();
		pObj->CreateContents(BOW1);
		pObj->CreateContents(ARWP);
		pObj->CreateContents(ARWP);
	}
	
	Object(4394)->SetAI();
	Object(4394)->Redefine(KARH);
	Object(4394)->InitType();
	Object(4393)->SetAI();
	Object(4393)->Redefine(KARH);
	Object(4393)->InitType();
//	SetNext("Script1", 70);

  if(pMessenger) pMessenger->RemoveObject();
	pMessenger = CreateObject(KKGT, 32, 503+10, 10);//257, 539+10, 10);
	pMessenger->SetAI();
	pMessenger->SetPhysical("Energy", 100000, 2);
	pMessenger->DoEnergy(100);
	pMessenger->SetDir(1);
	pMessenger->AttachLight();

	pMessenger->LocalN("ai_fRunner") = 1;
	pMessenger->LocalN("ai_fCallHome") = 1;

	pMessenger->AI_AddPatrolPoint(1869, 468);
	PlaceSunLight();
	CallOnCondition("MessengerDead", this, "CheckMessengerDead");
	iStory = 0;
}

local iStory;

func CheckMessengerDead() { if(!GetAlive(pMessenger)) return true; }

func MessengerDead()
{
	if(iStory > 1) return;
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		GetCursor(GetPlayerByIndex(i, C4PT_User))->OpenRelauchMenu("Der Bote ist gestorben!");
}

func AI_HomeReached(pTarget)
{
	if(iStory == 0)
	{
		iStory = 1;
		return;
	}
	if(iStory == 1)
	{
		pMessenger->AI_ResetPatrol();
		pMessenger->AI_AddPatrolObject(FindObject(CAVE));
		pMessenger->SetComDir(COMD_Stop);
		pMessenger->LocalN("ai_iDisableTime") = 35;
		iStory = 2;
		Object(3223)->Activate();
//		Log("Reached1");
		return 1;
	}
	if(iStory == 2)
	{
		Object(3223)->Activate();
		var fInside = 0;
/*		for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		{
			var pObj = GetCursor(GetPlayerByIndex(i, C4PT_User));
			if(Inside(pObj->GetY(), 400, 500)) // In der Höhle
				if(GetX(pObj)>GetX(Object(3223))) // Hinter dem Tor
					fInside = 1;
		}*/
		if(!fInside)
			for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
				GetCursor(GetPlayerByIndex(i, C4PT_User))->OpenRelauchMenu("Der Bote ist geflohen!");// und du bist ausgesperrt!");
//		Log("Reached2");
		pMessenger->RemoveObject();
		iStory = 3;
		return 1;
	}
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