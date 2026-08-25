/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; SetPosition(0,0); }

protected func MenuQueryCancel() { return 1; }

local father;
local pClonk;

local attackers;
local attacker_count;

static g_StoryIndex;

func HideAttackers()
{
	if(attacker_count) return;
	attackers = [];
	attacker_count = 0;

	// Die Räuber
	Hide(Object(886));
	Hide(Object(864));
	Hide(Object(905));
	Hide(Object(865));
	Hide(Object(906));

	// Beide Kanderianer
	Hide(Object(907));
	Hide(Object(908));
}

func Hide(obj)
{
	SetObjectStatus(C4OS_INACTIVE, attackers[attacker_count++] = obj);
}

func ShowAttackers()
{
	for(var obj in attackers)
	{
		SetObjectStatus(C4OS_NORMAL, obj);
		var color = obj->GetColorDw();
		obj->SetOwner(10);
		obj->SetColorDw(color);
	}
	attacker_count = 0;
}

func OnSave()
{
	if(attacker_count)
	{
		ShowAttackers();
		attacker_count = -1;
	}
}

func OnRestore()
{
	if(attacker_count == -1)
	{
		attacker_count = 0;
		HideAttackers();
	}
}

func Start()
{
	DoPlayMusic("The Pyre.ogg");
	ScheduleCall(this, "DoStart", 1, 1);
}

func DoStart()
{
	// Starte in einer anderen Section
  SaveObjects();
  LoadScenarioSection("Map", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var pObj = GetCursor(GetPlayerByIndex(i, C4PT_User));
		SetPosition(1247, 316, pObj);
		pObj->SetDir(0);
	}

	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));

	father = Object(876);
	father->MakeOlrog();
	HideAttackers();

	SetDirToPermanent(pClonk, father);

	StartFilm();

	PosCam(father, 0, 1);
  g_pCamera->SetPlrViewRange(40);

	FilmFadeDark();

	DialogMessage([BO69, "Buch Harkon, Kapitel 1", "1"], "Es ist die Zeit des großen Kanderianischen Krieges. In einem weit abgelegenen Tal wohnt Harkon bei seinem Vater Olrog. Doch das sorglose Leben dort wird sich bald ändern...", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	DoPlayMusic(["Midnight Meeting.ogg", "The Forest and the Trees.ogg"]);
	SetNext("Text1", 1);
}

func Text1()
{
  DialogMessage(father, "Harkon mein Sohn, heute ist dein 18. Geburtstag. Ich muss dir heute eine wichtige Sache sagen.", "Text2", 0, 0, "Dialog2.ogg");
  return 1;
}

func Text2(idID, pClonk)
{
  DialogMessage(father, "Auch wenn unser bescheidenes Tal wenig abwirft, bist du zu weit mehr bestimmt.", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3(idID, pClonk)
{
  DialogMessage(father, "Ich werde dir heute Abend alles bei Einbruch der Dunkelheit erklären. Aber vorher nutze den Tag um etwas Holz hacken zu gehen, unser Vorrat ist erschöpft.", "EndIntro", "Starten", 0, "Dialog4.ogg");
}

func Nothing() { return; }

func EndIntro()
{
	StopFilm();
	StartSzen();
}

//------------------------ Intro Ende ----------------------

func DoStartSzen()
{
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));

	father = Object(876);

	HideAttackers();

	SetDirToPermanent(pClonk, father);

	SetNext("Script10", 10);
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

func Script10()
{
	var found = 0;
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var iPlr = GetPlayerByIndex(i,C4PT_User);
		if(GetCursor()->GetAction() == "Chop")
			found = 1;
	}
	if(!found) SetNext("Script10", 1);
	else SetNext("Script30", 10);
}

func Script30()
{
	var found = 0;
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var iPlr = GetPlayerByIndex(i,C4PT_User);
		if(GetCursor()->GetAction() == "Chop")
			found = 1;
	}
//	if(!found) SetNext("Script10", 5);
	ShowAttackers();
	DisableAI();
	father->Kill();
	father->MakeOlrog();
	Object(1050)->RemoveObject();
	Object(1018)->Incinerate(); // Hut
	Object(1018)->Extinguish();
	AddEffect("IntSmokeHut", Object(1018), 1, 5, this);
	Object(881)->Incinerate();// Windmill
	Object(881)->Extinguish();
	AddEffect("IntSmokeWindmill", Object(881), 1, 5, this);
	Object(883)->RemoveObject();//Windmill wheel

	// Hut and Windmill
	AddEffect("NoBurning", Object(1018), 200, 0);
	AddEffect("NoBurning", Object(881), 200, 0);

	Sound("Blast3");
	Sound("Blast1");

	StopClonkEx(pClonk);
	DoPlayMusic("Final Count.ogg");
	DialogMessage(pClonk, "Was ist geschehen? Da hinten steigt Rauch auf! Ich muss sofort zurück!", "Script30b", 0, 0, "Dialog5.ogg");
	
	// Alle Clonks müssen links sein:
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var iPlr = GetPlayerByIndex(i,C4PT_User);
		if(GetCursor(iPlr)->GetX() > LandscapeWidth()/2)
			GetCursor(iPlr)->SetPosition(pClonk->GetX(), pClonk->GetY());
	}
	
	g_StoryIndex = 1;
}

global func FxNoBurningEffect(szNewEffect, iEffectTarget, iEffectNumber, iNewEffectNumber, var1, var2, var3)
  {
  // Feuer abblocken
  if (WildcardMatch(szNewEffect, "*Fire*")) return -1;
  // Alles andere ist OK
  return;
  }

func Script30b()
{
	CallOnCondition("EnemySeen", this, "CheckClonkNearAttackers");
}

// 868, 283
func CheckClonkNearAttackers(){
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var iPlr = GetPlayerByIndex(i,C4PT_User);
		if(GetCursor()->GetX() >= 940)
			return 1;
	}
}

func EnemySeen()
{
	StopClonkEx(pClonk);
	SetCommand(pClonk, "MoveTo", 0, 868, 283);
	DialogMessage(pClonk, "Oh nein, da sind ja Räuber! Ich muss sie irgendwie austricksen. Vielleicht kann ich mir am Sägebock aus Holz eine Falle basteln. Für den Notfall hatte Vater auch irgendwo noch zwei Brote versteckt.", "EnemySeenb", 0, 0, "Dialog6.ogg");
	for(var i = 0; i < GetPlayerCount(); i++)
	  SetPlrKnowledge(GetPlayerByIndex(i), BRTR);
	g_StoryIndex = 2;
}

func EnemySeenb()
{
	DialogMessage([_SAV, "Hinweis"], "<i>Bevor es brenzlig wird kann man im Kontextmenü das Schnellspeichern verwenden.</i>", "EnemySeenC", 0, 0, "Dialog7.ogg");
}

func EnemySeenC()
{
	DialogMessage([BRTR, "Hinweis"], "<i>Die Falle kann am Sägebock gebaut werden. Wie ein Brückensegment wird sie über ein Loch geschoben und bricht durch, wenn Gegner drüber laufen.|Tipp: man kann auch Flints oder Zapnester drauflegen.</i>", "Script40", 0, 0, "Dialog8.ogg");
}

func Script40()
{
	EnableAI();
	SetNext("Script50", 20);
}

func FxIntSmokeHutTimer(target) { target->Smoke(RandomX(-20,20),20, 10+Random(30), RGB(64,64,64)); }
func FxIntSmokeWindmillTimer(target) { target->Smoke(RandomX(-5,5),20-Random(20), 10+Random(20), RGB(64,64,64)); }

func Script50()
{
	if(FindObject2(Find_ID(_RBR), Find_OCF(OCF_Alive))) return SetNext("Script50", 5);

	StartFilm();

	PosCam(pClonk, 0, 1);
  g_pCamera->SetPlrViewRange(40);

	StopClonkEx(pClonk);
	DoPlayMusic("Epic Unease.ogg");
	DialogMessage(pClonk, "Vater, Vater, was ist mit dir? Lebst du noch?", "Text4a", 0, 0, "Dialog9.ogg");
	SetCommand(pClonk, "MoveTo", Object(876));
}

func Text4a()
{
	StopClonkEx(pClonk);
	DialogMessage(father, "...mein Sohn... ich muss dir sagen...aaargh", "Text4b", 0, 0, "Dialog10.ogg");
	pClonk->SetPosition(Object(876)->GetX(), Object(876)->GetY());
}

func Text4b()
{
	StopClonkEx(pClonk);
	DialogMessage(pClonk, "Diese widerlichen Mistkerle, sie haben meinen Vater ermordet! Wie soll es denn jetzt nun weitergehen? Und was wollte er mir eigentlich noch so wichtiges sagen?", "Text4c", 0, 0, "Dialog11.ogg");
}

func Text4c()
{
	StopClonkEx(pClonk);
	DialogMessage(pClonk, "Ich schwöre bei Sawelô, ich werde dich rächen Vater! Ich finde die Schweine, die den Überfall befohlen haben!", "Text5", 0, 0, "Dialog12.ogg");
}

local pMage;

func Text5()
{
	pMage = CreateObject(MAGE, 1304, 217+10, -1);
	pMage->MakeJuncus();
	pMage->CastObjects(SPRK, 10, 20);
	pMage->SetPlrViewRange(10);

	SetDirTo(pMage, pClonk);
	SetDirTo(pClonk, pMage);
	DialogMessage(pMage, "Was geht hier vor? Wurdet ihr überfallen?", "Text6", 0, 0, "Dialog13.ogg");
}

func Text6()
{
	StopClonkEx(pClonk);
	DialogMessage(pClonk, "Diese Räuber haben unseren Hof überfallen und meinen Vater getötet. Ich konnte meinen Vater allerdings rächen und diese verlogenen Mistkerle besiegen! Doch was soll jetzt aus mir werden?", "Text7", 0, 0, "Dialog14.ogg");
}

func Text7()
{
	DialogMessage(pMage, "Oh wie tragisch. Das tut mir Leid. Komm mit mir, ich gewähre dir Obdach und wir können vielleicht rausfinden, wer hinter diesem Überfall gesteckt hat.", "Text8", "Ende", 0, "Dialog15.ogg");
}

func Text8()
{
	FilmFadeOut();
	SetNext("End", 4);
}

func End()
{
	FinishMission();
}
