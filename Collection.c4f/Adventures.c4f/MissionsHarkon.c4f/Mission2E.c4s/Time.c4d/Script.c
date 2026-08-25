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

	DialogMessage([BO69, "Buch Harkon, Kapitel 2", "1"], "Nachdem Harkon mit Marc nur knapp von Fenrings Wachen fliehen konnte, bringt er nun Marc zurück nach Rockfort.", "Text0", 0, 0, "Dialog1.ogg");
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
  DialogMessage(pGuard, "Da kommt Harkon mit dem Luftschiff zurück!", "Text2", 0, 0, "Dialog2.ogg");
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
	DialogMessage(pMage, "Willkommen zurück Marc, ich hatte mir schon große Sorgen gemacht.", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
  DialogMessage(pClonk, "Ja puh, das war echt knapp, fast hätten sie uns auf der Flucht noch erwischt.", "Text4", 0, 0, "Dialog4.ogg");
}

func Text4()
{
  DialogMessage(pMage, "Marc, erzähl doch mal was passiert ist, warum konnte Fenring dich festnehmen?", "Text5", 0, 0, "Dialog5.ogg");
}

func Text5()
{
  DialogMessage(pMarc, "Es war eine Falle. Fenring hatte mich schon erwartet, ich konnte nichts machen.", "Text6", 0, 0, "Dialog6.ogg");
	pClonk->SetDir(0);
}

func Text6()
{
  DialogMessage(pMage, "Sehr beunruhigend. Wir müssen unbedingt mehr herausfinden.", "Text7", 0, 0, "Dialog7.ogg");
	pClonk->SetDir(1);
}

func Text7()
{
  DialogMessage(pMage, "Marc, ruh dich erstmal aus. Harkon, du musst sofort nochmal zurück zur Burg von Fenring und versuchen mehr herauszufinden. Wir müssen unbedingt an den Grafen rankommen.", "Text8", 0, 0, "Dialog8.ogg");
}

func Text8()
{
  DialogMessage(pClonk, "Was? Ich bin die ganze Nacht durch hergeflogen und soll jetzt schon wieder aufbrechen? Was soll das? Kann das nicht Fred oder Malvin machen?", "Text9", 0, 0, "Dialog9.ogg");
}

func Text9()
{
  DialogMessage(pMage, "Ihr beide erzählt am besten erstmal keinem hier davon, dass es eine Falle war, denn vielleicht haben wir einen Spion unter uns, der ihn vorher informiert hat. Tut einfach so, als wäre es dein Fehler gewesen, Harkon!", "Text10", 0, 0, "Dialog10.ogg");
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
  DialogMessage(pMarc, "Tu am besten was er gesagt hat. Aber nach dem was ich von Fenrings Wachen belauschen konnte, hat sich Fenring längst in ein geheimes Versteck in den Bergen abgesetzt. Sein Magier hält den Zugang dazu mit einem starken Zauber verborgen.", "Text12", 0, 0, "Dialog11.ogg");
	pClonk->SetDir(0);
}

func Text12()
{
  DialogMessage(pClonk, "Ok, dann fliege ich wohl mal wieder zurück und knöpfe mir diesen Magier vor...", "Text13", 0, 0, "Dialog12.ogg");
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
  DialogMessage(pClonk, "Nun muss ich wohl nochmal Fenrings Burg infiltrieren und seinen Hofmagier ausschalten, der mit seinem Zauber das Versteck von Fenring schützt.", "EndIntro", 0, 0, "Dialog13.ogg");
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
  DialogMessage(pClonk, "So, der Magier liegt im Sterben. Jetzt aber schnell weg von hier.", "Nothing", 0, 0, "Dialog14.ogg");
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