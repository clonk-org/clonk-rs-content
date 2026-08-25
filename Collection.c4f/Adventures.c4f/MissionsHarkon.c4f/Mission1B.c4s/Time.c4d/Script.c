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

	DialogMessage([BO69, "Buch Harkon, Kapitel 1", "1"], "Harkon begleitet Juncus von seinem abgelegenen Heimattal zur versteckten Burg Rockfort, die fortan sein Zuhause sein wird...", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
}

func Text1()
{
  // Auftrag
  DialogMessage(pMage, "Das hier wird dein Zimmer auf der Burg Rockfort sein. Sie steht im Schutze des Rates von Cla-Kra-Zoth und das Hauptquartier von Cras Geheimbund.", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	DialogMessage(pClonk, "Geheimtruppe? Was macht die und was habe ich damit zu tun? Und wer waren die Mörder meines Vaters?", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
	DialogMessage(pMage, "Wer deinen Vater überfallen hat weiß ich noch nicht. Aber du wirst hier ausgebildet werden und an unserer Seite kämpfen. Gleich morgen geht es los mit deiner ersten Lektion, in der du lernst dich anzuschleichen, was sehr wichtig für dich sein wird.", "Text3b", 0, 0, "Dialog4.ogg");
}

func Text3b()
{
	DialogMessage(pClonk, "Ich verstehe das alles nicht. Das ist irgendwie noch zu viel für mich.", "Text4", 0, 0, "Dialog5.ogg");
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
	DialogMessage(pMage, "Hier sind die Zimmer deiner Mitschüler Fred und Malvin. Du wirst dich mit ihnen gut verstehen, da sie auch erst vor kurzem angefangen haben. Sie geben dir bestimmt Rückhalt und helfen dir.", "Text6", 0, 0, "Dialog6.ogg");
}

func Text6()
{
	pMage->SetDir(1);
	DialogMessage(pMage, "Aber sie schlafen gerade, sie hatten einen anstrengenden Tag. Du solltest dich auch ausruhen für deine erste Lektion morgen.", "Text7", 0, 0, "Dialog7.ogg");
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
	DialogMessage(pMage, "Guten Morgen Harkon. Hier ist deine erste Lektion. Du sollst dich in das Lager schleichen. [Tarnen im Kontextmenü aktivieren]", "In2", 0, 0, "Dialog8.ogg");
}

func In2()
{
	DialogMessage(pMage, "Dort musst du dann alle Clonks mit der Keule betäuben [Doppelgraben hinter dem Opfer]. Wenn du fertig bist, komm einfach zum Wegweiser zurück.", "In3", 0, 0, "Dialog9.ogg");
}

func In3()
{
	pMage->PoffClonk();
	pMage->RemoveObject();
	DialogMessage(pClonk, "Hmm na gut, dann bleibt mir wohl nichts anderes übrig, als das mal zu versuchen.", "InEnd", "Start", 0, "Dialog10.ogg");
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
	DialogMessage(pClonk, "So, alle sind betäubt. Jetzt muss ich nur noch zurück zum Wegweiser [Anfassen+Oben]", "Nothing", 0, 0, "Dialog11.ogg");
}

// ----------------------- Outro -----------------------

public func SignpostReached()
{
	DialogMessage(pClonk, "Puh, das ist garnicht so einfach, was dieser Juncus da von mir verlangt. Ich frage mich, was das alles zu bedeuten hat und was das mit dem Überfall zu tun hat.", "Out1", 0, 0, "Dialog12.ogg");
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
	DialogMessage(pMage, "Sehr gut Harkon, das war für den Anfang garnicht mal so schlecht. Als nächstes muss du dann lernen mit scharfen Waffen umzugehen. Aber damit fangen wir erst morgen an.", "OutEnd", "Ende", 0, "Dialog13.ogg");
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