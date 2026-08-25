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

	DialogMessage([BO69, "Buch Harkon, Kapitel 1", "1"], "Doch Harkon ist nicht alleine auf Burg Rockfort. Fred und Malvin werden ebenfalls auf der Burg ausgebildet...", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
}

func Text1()
{
	DialogMessage(pMalvin, "Boah, ich halte es langsam nicht mehr aus in dieser Kerkerzelle. Mein Bettzeug ist auch schon leicht schimmlig.", "Text2", 0, 0, "Dialog2.ogg");
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
	DialogMessage(pFred, "Naja, meines ist auch nicht gerade besser. Mich starrt immer so ein Schädel an, wenn ich im Bett liege.", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
	pMalvin->SetDir(0);

	DialogMessage(pClonk, "Wisst ihr eigentlich, dass man da unten kaum schlafen kann, wenn ihr über einem redet? Außerdem habe ich sowieso wenig geschlafen, weil ich von diesen grässlichen Todeskrähen geträumt hab.", "Text4", 0, 0, "Dialog4.ogg");
}

func Text4()
{
	DialogMessage(pMalvin, "Ach sieh an, der neue! Du bist also der mysteriöse neue Lehrling, den Juncus eigenhändig unterrichtet.", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
	DialogMessage(pFred, "Jep, vom Viceboss persönlich unterrichtet zu werden ist schon eine Ehre. Das solltest du zu schätzen wissen.", "Text6", 0, 0, "Dialog6.ogg");
}

func Text6()
{
	DialogMessage(pMalvin, "Wir mussten es uns hart erkämpfen, hier aufgenommen zu werden und trotzdem werden wir nur von Marc unterrichtet.", "Text7", 0, 0, "Dialog7.ogg");
}

func Text7()
{
	DialogMessage(pClonk, "Ich habe mir das nicht ausgesucht, ich bin da hineingerutscht... Außerdem muss ich jetzt los zu meinem Training.", "Text8", 0, 0, "Dialog8.ogg");
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

	DialogMessage(pFred, "Irdendwas ist komisch an ihm. Warum hat er nur so eine Sonderrolle hier?", "End", "Ende", 0, "Dialog9.ogg");
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
	DialogMessage(pMage, "Wie ich höre hast du dich auf der Burg mit Fred und Malvin angefreundet.", "In2", 0, 0, "Dialog10.ogg");
}

func In2()
{
	DialogMessage(pClonk, "Angefreundet habe ich mit denen noch lange nicht. Dann wissen sie allerdings bestimmt auch, dass ich schlecht geschlafen habe und heute nicht trainieren will.", "In3", 0, 0, "Dialog11.ogg");
}

func In3()
{
	DialogMessage(pMage, "Die Armbrust kann auch einen Enterhaken mit Seil verschießen. Damit kannst du an viele sonst unerreichbare Stellen klettern. Wie zum Beispiel der Wegweiser auf der anderen Seite des Canyons.", "In4", 0, 0, "Dialog12.ogg");
}

func In4()
{
	DoWealth(GetOwner(pClonk), 5);
	pClonk->Sound("Cash");
	DialogMessage(pMage, "Du darfst dabei aber nicht gesehen werden. Zur Ablenkung der Wachen können diese Münzen hilfreich sein, du kannst sie werfen um Gegner abzulenken [Kontextmenü].", "In5", 0, 0, "Dialog13.ogg");
}

func In5()
{
	pMage->PoffClonk();
	pMage->RemoveObject();
	DialogMessage(pClonk, "Poff und weg ist er... Dann muss ich mich wohl mal trotz Müdigkeit mit diesem Enterhaken anfreunden.", "InEnd", "Start", 0, "Dialog14.ogg");
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
	DialogMessage(pClonk, "Gähn, puh, das war ja eine Tortur mit der Kletterei. So Juncus, ich bin fertig!", "Out1", 0, 0, "Dialog15.ogg");
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
	DialogMessage(pMage, "Ja, so ein Enterhaken ist schon was praktisches, nicht wahr?", "Out2", 0, 0, "Dialog16.ogg");
}

func Out2()
{
	DialogMessage(pClonk, "Ja Meister Juncus.", "Out3", 0, 0, "Dialog17.ogg");
}

func Out3()
{
	DialogMessage(pMage, "Sehr gut. Deine ersten Trainingsmissionen haben Erfolg gezeigt. Deswegen wird dich morgen Marc in die Geheimnisse unserers Geheimbundes einweihen. Finde dich dazu morgen in der Bibliothek ein. Uns zwar um 6 Uhr früh.", "OutEnd", "Ende", 0, "Dialog18.ogg");
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