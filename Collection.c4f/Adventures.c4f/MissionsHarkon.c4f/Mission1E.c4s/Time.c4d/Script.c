/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; SetPosition(0,0); }

protected func MenuQueryCancel() { return 1; }

local pMarc;
local pClonk;
local pFred;
local pMalvin;
local pMage;
local pCra;

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
	
	for(var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_PlayerType(C4PT_Script)))
		obj->SetPlrViewRange(0);

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	StartFilm();
	
	pFred = CreateObject(ASAS, 1268, 421+10, -1);
	pFred->MakeFred();
	pFred->SetDir(1);
	pFred->SetAction("Idling");

	pMalvin = CreateObject(ASAS, 1288, 421+10, -1);
	pMalvin->MakeMalvin();
	pMalvin->SetDir(0);
	pMalvin->SetAction("Idling");
	pMalvin->SetPhase(8);

	pMarc = CreateObject(ASAS, 1166, 420+10, -1);
	pMarc->MakeMarc();

	pMage = CreateObject(MAGE, 1194, 521+10, -1);
	pMage->MakeJuncus();
	Object(1427)->ProductionStart(pMage, NOTH);
	Object(1427)->SetColorDw(pMage->GetColorDw());

//	pMalvin->SetPlrViewRange(40);

	pClonk->SetPosition(1322, 422);
	
	SetDirTo(pFred, pMalvin);
	SetDirTo(pMalvin, pFred);
	SetDirTo(pMalvin, pClonk);

	PosCam(pMalvin, 0, 1);
  g_pCamera->SetPlrViewRange(40);
	FilmFadeDark();

	DialogMessage([BO69, "Buch Harkon, Kapitel 1", "1"], "Harkons Grundausbildung geht in die letzte Runde. Er steht kurz davor in den Geheimbund offiziell aufgenommen zu werden...", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
}

func Text1()
{
	DialogMessage(pMalvin, "Marc hat uns also hier in die Bibliothek herbestellt. Was soll denn das schon wieder...", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	DialogMessage(pFred, "Wahrscheinlich will er uns was von dem geschichtlichen Zeugs erzählen. Das bringt uns bei unseren Aufträgen bestimmt sehr viel weiter.", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
	DialogMessage(pClonk, "Achtung, da kommt er!", "Text4", 0, 0, "Dialog4.ogg");
	// Marc kommt zur Tür rein
	Object(1411)->DoOpen();
	Object(1411)->ScheduleCall(0, "DoClose", 35);
	SetCommand(pMarc, "MoveTo", 0, 1230, 420);
	// Harkon stellt sich mehr zu den anderen
	SetCommand(pClonk, "MoveTo", 0, 1310, 422);
	PosCam(pMarc);
	SetDirTo(pMarc, pFred);
	SetDirTo(pMarc, pMalvin);
}

func Text4()
{
	DialogMessage(pMarc, "In der Tat. Ich will euch ein paar Hintergründe erläutern. Es ist nämlich immer sehr wichtig zu wissen, für was man eigentlich kämpft.", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
	DialogMessage(pMarc, "In Meduvien herrscht zur Zeit Krieg. Die Truppen des Königs mit der Unterstützung der Kirchen kämpfen gegen das Heer der Kanderianer.", "Text6", 0, 0, "Dialog6.ogg");
}

func Text6()
{
	DialogMessage(pFred, "Aber man munkelt, dass es eine Gruppe Rebellen geben soll, die versuchen dem König in den Rücken zu fallen und den Krieg so zu beenden!", "Text7", 0, 0, "Dialog7.ogg");
	pFred->SetAction("Walk");
	pFred->JumpUp();
}

func Text7()
{
	DialogMessage(pMarc, "Genau, der König schert sich jedoch nicht um die Rebellion, da er mit dem Kanderianern selbst genug zu tun hat. Hier kommt jetzt der Magierrat von Cra-Kla-Zoth ins Spiel.", "Text8", 0, 0, "Dialog8.ogg");
}

func Text8()
{
	SetDirTo(pMalvin, pFred);
	SetDirTo(pMalvin, pClonk);
	DialogMessage(pMalvin, "Und Cra ist einer der drei Vorsitzenden dieses Rates und führt diesen Geheimbund an.", "Text9", 0, 0, "Dialog9.ogg");
}

func Text9()
{
	SetDirTo(pMarc, pFred);
	SetDirTo(pMarc, pClonk);
	DialogMessage(pMarc, "Ja, so ist es. Über Cras Geheimbund versucht der Rat die Rebellen mit ihren eigenen Waffen zu schlagen. Gezielte hinterhältige Attentate um die Rebellen auszuschalten und innere Sicherheit zu gewährleisten.", "Text10", 0, 0, "Dialog10.ogg");
}

func Text10()
{
	SetDirTo(pClonk, pFred);
	SetDirTo(pClonk, pMalvin);
	DialogMessage(pClonk, "Aha, also werden wir dann zu hinterhältigen Meuchlern ausgebildet und sollen dann für Frieden und Sicherheit kämpfen, wie es der Rat für Meduvien will.", "Text11", 0, 0, "Dialog11.ogg");
}

func Text11()
{
	SetDirTo(pMarc, pFred);
	SetDirTo(pMarc, pClonk);
	SetDirTo(pMarc, pMalvin);
	DialogMessage(pMarc, "Ja genau. Aber jetzt genug der Theorie, ich seh schon ihr werdet unruhig. Harkon suche Juncus auf. Fred und Malvin, euch werde ich zum nächsten Auftrag einweisen.", "Text12", 0, 0, "Dialog12.ogg");
}

func Text12()
{
	PosCam(pMage);
	Object(1411)->ScheduleCall(0, "DoOpen", 35);
	Object(1411)->ScheduleCall(0, "DoClose", 35*2);
	AddCommand(pClonk, "Call", this, 0, 0, 0, 0, "Text13");
	AddCommand(pClonk, "Grab", Object(1416));
}

func Text13()
{
	Object(1416)->SetMoveTo(500);
	SetNext("Text14", 1);
//	AppendCommand(Object(1416), "Call", this, 0, 0, 0, 0, "Text14");
}

func Text14()
{
	Object(1421)->ScheduleCall(0, "DoOpen", 25);
	Object(1421)->ScheduleCall(0, "DoClose", 35*2);
	AddCommand(pClonk, "Call", this, 0, 0, 0, 0, "Text15");
	AddCommand(pClonk, "MoveTo", 0, 1155, 500);
}

func Text15()
{
	Object(1427)->Exit(pMage, 7, 9);
	Object(1427)->SetAction("Idle");
	pMage->SetDir(0);
	DialogMessage(pMage, "Ach Harkon, da bist du ja schon.", "Text16", 0, 0, "Dialog13.ogg");
}

func Text16()
{
	DialogMessage(pClonk, "Ja und wie es aussieht soll ich als gemeiner Meuchelmörder ausgebildet werden und für Cra Leute töten!", "Text17", 0, 0, "Dialog14.ogg");
}

func Text17()
{
	DialogMessage(pMage, "Hast du dir das noch nicht gedacht, während deiner Ausbildung? Du wirst für den Rat kämpfen und zusammen mit uns die Rebellion niederschlagen. Willst du das nicht?", "Text18", 0, 0, "Dialog15.ogg");
}

func Text18()
{
	pClonk->JumpUp();
	DialogMessage(pClonk, "Ich will meinen Vater rächen und nicht die Befehle von diesem Cra ausführen! Ich habe nie gesagt, dass ich ein Mörder werden will.", "Text19", 0, 0, "Dialog16.ogg");
}

func Text19()
{
	DialogMessage(pMage, "Mörder ist ein hartes Wort. Mit deiner Ausbildung wirst du bestens dafür vorbereitet sein, Rache zu üben.", "Text20", 0, 0, "Dialog17.ogg");
}

func Text20()
{
	DialogMessage(pMage, "Heute erwartet dich aber zunächst dein erster richtiger Auftrag, nicht nur mit Trainingsgegnern. Ich werde dir dein Ziel auf der Karte zeigen.", "End", "Ende", 0, "Dialog18.ogg");
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
  pClonk = GetHiRank(GetPlayerByIndex(0, C4PT_User));
  pClonk->Hide(1, 1, 1);
  SaveObjects();
	LoadScenarioSection("Map", 0);
  // Alles wiederherstellen
  RestoreSectObjs();
	StartSzen();
	DoPlayMusic("Final Count.ogg");
	DialogMessage(pClonk, "Das hier mus also der Außenposten sein, den mir Juncus auf der Karte gezeigt hat. Dort soll ich alle Goldreserven stehlen und dann heil zurück kommen. Hoffentlich muss ich nicht Gebrauch von diesen Wurfmessern hier machen und jemanden umbringen.", "Nothing", "Start", 0, "Dialog19.ogg");
}

func DoStartSzen()
{
	if(g_fIntroSkiped) DoPlayMusic("Final Count.ogg");

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	CallOnCondition("ScriptFinished", this, "ScriptFinishedCondition");
	//Test
/*	pClonk->CreateContents(ZAPN);
	pClonk->CreateObject(BRTR);*/
}

func ScriptFinishedCondition() { return GameCall("IsFullfilled"); }

func ScriptFinished() {
	DialogMessage(pClonk, "Yeah, das war alles Gold! Nun muss ich nurnoch hier rauskommen und zum Wegweiser zurück.", "Nothing", 0, 0, "Dialog20.ogg");
}

public func SignpostReached()
{
	StartFilm();
	PosCam(pClonk, 0, 1);
	DialogMessage(pClonk, "Ok, alles Gold mitgenommen. Jetzt kommt bestimmt gleich der Juncus mit einem Plopp wieder...", "Out1", 0, 0, "Dialog21.ogg");
	return 1;
}

func Out1()
{
	DialogMessage(pClonk, "Hmm, sieht wohl so aus, also ob ich selber nach Rockfort zurücklaufen muss.", "Out2", 0, 0, "Dialog22.ogg");
}

func Out2()
{
	FilmFadeOut();
	SetNext("Out3", 4);
}

func Out3()
{
	StopFilm();
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Rockfort", 0);
  // Alles wiederherstellen
  RestoreSectObjs();

	pClonk->UnHide();

	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		GetCrew(GetPlayerByIndex(i, C4PT_User))->SetPlrViewRange(0);
	
	SetPosition(0,0);
	
	StartFilm();

	pCra = CreateObject(MAGE, 652, 371+10, -1);
	pCra->MakeCra();

	pMage = CreateObject(MAGE, 596,371+10, -1);
	pMage->MakeJuncus();

	pClonk->Enter(Object(307));

	SetDirTo(pMage, pCra);
	SetDirTo(pCra, pMage);

	PosCam(pCra, 0, 1);
	g_pCamera->SetPlrViewRange(40);

	FilmFadeIn();
	DoPlayMusic("Epic Unease.ogg");
	SetNext("OutB1", 1);
}

func OutB1()
{
  // Auftrag
  DialogMessage(pCra, "Und du bist dir sicher, das er der richtige ist?", "OutB2", 0, 0, "Dialog23.ogg");
}

func OutB2()
{
	DialogMessage(pMage, "Ja, er ist es wirklich, da bin ich mir ganz sicher.", "OutB3", 0, 0, "Dialog24.ogg");
}

func OutB3()
{
	AddCommand(pClonk, "MoveTo", 0, 562, 372);
	SetDirTo(pClonk, pMage);
	DialogMessage(pClonk, "Juncus, ihr habt mich herbestellt.", "OutB4", 0, 0, "Dialog25.ogg");
}

func OutB4()
{
	DialogMessage(pMage, "Ja, dein Training ist abgeschlossen. Tritt nun vor unseren Meister, Cra.", "OutB5", 0, 0, "Dialog26.ogg");
}

func OutB5()
{
	AddCommand(pClonk, "MoveTo", 0, 622, 372);
	SetDirToPermanent(pClonk, pMage);
	SetNext("OutB6", 1);
}

func OutB6()
{
	DialogMessage(pCra, "Harkon, du bist nun würdig in unsere Gemeinschaft aufgenommen zu werden. Ich erhebe dich hiermit in den Rang eines Adepten.", "OutB7", 0, 0, "Dialog27.ogg");
}

func OutB7()
{
	pClonk->CastObjects(SPRK, 10, 20);
	pClonk->SetGraphics("Adept");
	pClonk->SetPortrait("Adept1");
	pCra->SetAction("Magic");
	SetNext("OutB8",2);
}

func OutB8()
{
	DialogMessage(pCra, "Und nun geh und ruh dich erstmal aus. Es warten viele Herrausforderungen auf dich.", "OutB9", 0, 0, "Dialog28.ogg");
}

func OutB9()
{
	SetCommand(pClonk, "Enter", Object(307));
	FilmFadeOut();
	SetNext("OutB10", 4);
}

func OutB10()
{
	DialogMessage([BO69, "Buch Harkon, Kapitel 1", "1"], "So schloss Harkon sein Training ab und wurde offiziell in den Geheimbund aufgenommen, von dem er vor ein paar Wochen noch garnichts wusste. Eigentlich will er nur seinen Vater rächen und hätte am liebsten sein altes beschauliches Leben zurück.", "OutEnd", "Ende des Kapitels", 0, "Dialog29.ogg");
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