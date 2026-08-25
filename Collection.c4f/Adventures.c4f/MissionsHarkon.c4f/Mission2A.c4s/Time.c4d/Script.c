/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

local pMage;
local pCra;
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

	pCra = CreateObject(MAGE, 652, 371+10, -1);
	pCra->MakeCra();
	
	pMage = CreateObject(MAGE, 596,371+10, -1);
	pMage->MakeJuncus();
	
	pMalvin = CreateObject(ASAS, 821, 610+10, -1);
	pMalvin->MakeMalvin();
	
	pFred = CreateObject(ASAS, 851, 610+10, -1);
	pFred->MakeFred();

	pClonk->SetPosition(790, 610);
	pClonk->SetGraphics("Adept");
	pClonk->SetPortrait("Adept1");
	
	var pObj = CreateObject(CBRD, 778, 612, -1);
	pObj->SetCategory(1);
	pObj->SetObjectLayer(pObj);
	pObj = CreateObject(CBRD, 833, 613, -1);
	pObj->SetCategory(1);
	pObj->SetObjectLayer(pObj);
	pObj = CreateObject(GBLT, 757, 609, -1);
	pObj->SetCategory(1);
	pObj->SetObjectLayer(pObj);
	
	SetDirTo(pMage, pCra);
	SetDirTo(pCra, pMage);
	SetDirTo(pFred, pMalvin);
	SetDirTo(pFred, pClonk);
	SetDirTo(pClonk, pFred);
	
	for(var pObj in FindObjects(Find_ID(CNDL)))
		pObj->SetCategory(1);
	
	for(var pObj in FindObjects(Find_ID(TABL)))
		pObj->SetSolidMask();

	PosCam(pCra, 0, 1);
  g_pCamera->SetPlrViewRange(200);
	FilmFadeDark();

	DialogMessage([BO69, "Buch Harkon, Kapitel 2", "1"], "Harkon lebt nun schon seit einiger Zeit auf Burg Rockfort und hat sich mehr oder weniger mit seinem Schicksal arangiert.", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
}

func Text1()
{
  // Auftrag
  DialogMessage(pCra, "Wir müssen unbedingt etwas gegen Fenring unternehmen. Er unterstützt und stärkt die Rebellen immer weiter.", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
	DialogMessage(pMage, "Ja, wir müssten ihn aufhalten. Wobei, vielleicht kommen wir über ihn auch an andere Rebellen ran.", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
  DialogMessage(pCra, "Du meinst er hat vielleicht Aufzeichungen über andere Unterstützer der Rebellen? Das wäre großartig.", "Text4", 0, 0, "Dialog4.ogg");
}

func Text4()
{
	PosCam(pFred, 0, 0);
	SetNext("Text5", 2);
}

func Text5()
{
	DialogMessage(pFred, "Hey Harkon, kannst du mir vielleicht den Kelch da drüben rüberreichen?", "Text6", 0, 0, "Dialog5.ogg");
}

func Text6()
{
	SetDirTo(pClonk, pMalvin);
	pClonk->SetMacroCommand("MoveTo", pClonk->FindObject(GBLT, 0, 0, -1, -1));
	DialogMessage(pClonk, "Ja Fred, du bekommst deinen Kelch..", "Text7", 0, 0, "Dialog6.ogg");
}

func Text7()
{
	Fling(pClonk, 2,-2);
	pMage->SetPosition(742, 610);
	pMage->PoffClonk();
	SetNext("Text11", 2);
	SetDirToPermanent(pMage, pClonk);
	SetDirToPermanent(pMage, pFred);
	SetDirToPermanent(pMage, pMalvin);
}

func Text11()
{
	DialogMessage(pMage, "So, Finger weg vom Wein. Es wartet ein wichtiger Auftrag!", "Text13", 0, 0, "Dialog7.ogg");
}

func Text13()
{
	DialogMessage(pFred, "Oh ja ein Auftrag, den mach ich!", "Text14", 0, 0, "Dialog8.ogg");
	pFred->JumpUp();
}

func Text14()
{
	DialogMessage(pMage, "Nein! Harkon, DU wirst den Auftrag ausführen, komm mit.", "Text15", 0, 0, "Dialog9.ogg");
	pMage->SetAction("AimMagic");
	pMage->SetPhase(5);
}

func Text15()
{
	pMage->SetAction("Walk");
	DialogMessage(pFred, "Ach nein, immer bekommt er den Vorzug! Ich hasse es.", "Text16", 0, 0, "Dialog10.ogg");
	Object(1319)->ControlLeft(pMage);
	RemoveDirToPermanent(pClonk);
	pMage->SetMacroCommand("MoveTo", Object(1427));
	pClonk->SetMacroCommand("MoveTo", Object(1427));
	pClonk->AddMacroCommand("Wait", 35);
}

func Text16()
{
//	SetCommand(pClonk, "Enter", Object(307));
	FilmFadeOut();
	SetNext("EndIntro", 4);
}

func Nothing() { return; }

func End()
{
	FilmFadeOut();
	SetNext("EndIntro", 4);
}

local pMarc;

func EndIntro()
{
	StopFilm();
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Map", 0);
  // Alles wiederherstellen
  RestoreSectObjs();
	StartFilm();
	FilmFadeIn();

	pClonk->SetPosition(1777, 719);

	if(Object(1930)) Object(1930)->RemoveObject(); // Hofmagier
	
	pMarc = CreateObject(ASAS, 1615, 687+10, -1);
	pMarc->MakeMarc();

	SetDirTo(pClonk, pMarc);
	SetDirTo(pMarc, pClonk);

	pFenring = CreateObject(_CLK, 935, 220+10, 10);
	pFenring->~MakeFenring();

	pFenringMage = CreateObject(MAGE, 890, 230+10, 10);
	pFenringMage->~MakeFenringMage();

	SetDirTo(pFenringMage, pFenring);
	SetDirTo(pFenring, pFenringMage);
	
	PosCam(pFenring, 0, 1);
  g_pCamera->SetPlrViewRange(40);
	SetNext("TextB1", 1);
}

local pFenring;
local pFenringMage;

func TextB1()
{
	DialogMessage(pFenring, "So Maron, ich hoffe es ist alles vorbereitet?", "TextB2", 0, 0, "Dialog11.ogg");
}

func TextB2()
{
	DialogMessage(pFenringMage, "Ja Graf, es ist so wie sie es angeordnet haben. Wir sind bereit.", "TextB3", 0, 0, "Dialog12.ogg");
	pFenringMage->SetAction("Magic");
}

func TextB3()
{
	DialogMessage(pFenring, "Gut, dann werde ich mich auf den Weg machen, fangen wir an.", "TextB3b", 0, 0, "Dialog13.ogg");
}

func TextB3b()
{
  DialogMessage(pFenring, "Ach ja und kümmere dich mal um den Strom, der Notstromdynamo reicht hinten und vorne nicht für die Burg.", "TextB4", 0, 0, "Dialog14.ogg");
}

func TextB4()
{
	SetCommand(pFenring, "MoveTo", 0, 785, 230);
	PosCam(pMarc);
//  g_pCamera->SetPlrViewRange(40);
	SetNext("TextB5", 3);
}

func TextB5()
{
	pFenring->RemoveObject();
	pFenringMage->RemoveObject();
	DialogMessage(pMarc, "So Harkon. Bisher habe ich ja nur Fred und Malvin ausgebildet, aber heute werden wir einen Auftrag zusammen ausführen.", "TextB6", 0, 0, "Dialog15.ogg");
	pClonk->SetPlrViewRange(40);
}

func TextB6()
{
	DialogMessage(pMarc, "Cra meint, Graf Fenring besitzt das rote Buch der Rebellen. Dort sollen alle Aufzeichnungen über die Rebellen drin sein. Deswegen ist es sehr wichtig das Buch zu bekommen, um gegen die Rebellen vorgehen zu können.", "TextB7", 0, 0, "Dialog16.ogg");
}

func TextB7()
{
	DialogMessage(pMarc, "Fenrings Burg hat zwei Bibliotheken. Ich werde mir die obere vorknöpfen, du kümmerst dich bitte um die im Keller. Wir treffen uns dann wieder hier am Wegweiser.", "EndIntro2", 0, 0, "Dialog17.ogg");
}

func EndIntro2()
{
	StopFilm();
	StartSzen();
}

//------------------------ Intro Ende ----------------------

func DoStartSzen()
{
	DoPlayMusic(["Heart of Medieval.ogg", "For the Fallen.ogg"]);
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	if(!pMarc)
	{
		pMarc = CreateObject(ASAS, 0, 0, -1);
		pMarc->MakeMarc();
		pMarc->SetPosition(1615, 687);
	}
	var aUpperClonks = [1530,6935,1899,1883,1886,6930,6929,1869,1862,1861,4039,1876,1898,4139,1916,4144,1923,1932,9726,9725,9723,9724];
  for(var iClonk in aUpperClonks)
    if(Object(iClonk))
      LocalN("ai_iNoSpecial", Object(iClonk)) = 1;
	var pDynamo = FindObject(_DYN);
  pDynamo->SetAction("Working");
  pDynamo->SetColorDw(RGB(200,0,0));
  FindObject(_GDR)->RemoveObject();
	HideAttackers();
	
	SetNext("Script0", 0);
	
	CallOnPlayerNear("BibSeq1", FindObject(_FBO), this, 40);
}

local attackers;
local attacker_count;

func HideAttackers()
{
	if(attacker_count) return;
	attackers = [];
	attacker_count = 0;

  Hide(Object(10168));
	Hide(Object(3422));
	Hide(Object(3404));
	Hide(Object(3405));//Swordknight
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
	Object(3405)->LocalN("ai_iNoSpecial") = 1;
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

func Script0()
{
	SetCommand(pMarc, "Call", this, 0, 0, 0, "Script1");
	AddCommand(pMarc, "MoveTo", 0, 1412, 669);
}

func Script1()
{
	Sound("Crossbow*", pMarc);
	pMarc->SetPosition(1144, 530);
	pMarc->SetDir(1);
	CallOnSeen("Script1B", pMarc, this);
}

func Script1B()
{
	pMarc->SetAction("Assassasinate", Object(1530));
	SetNext("Script1C", 2);
}

func Script1C()
{
	SetCommand(pMarc, "Call", this, 0, 0, 0, "Script2");
	AddCommand(pMarc, "MoveTo", 0, 930, 540);
}

func Script2()
{
	Object(1599)->DoOpen(pMarc);
	SetCommand(pMarc, "Call", this, 0, 0, 0, "Script3");
	AddCommand(pMarc, "MoveTo", 0, 697, 470);
}

func Script3()
{
//	pMarc->RemoveObject();
}

func BibSeq1(pNewClonk)
{
	PlaceSunLight();
	ShowAttackers();
  Object(821)->DoClose(this);
	
	pClonk = pNewClonk;
	// Alle Clonks müssen links sein:
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		var iPlr = GetPlayerByIndex(i,C4PT_User);
		GetCursor(iPlr)->SetPosition(pClonk->GetX(), pClonk->GetY());
	}
	
	pMarc->SetPosition(880, 420);
	pMarc->SetDir(1);
	Object(1625)->SetAction("DoorOpen");
	
	
	StartFilm();
	PosCam(pMarc, 0, 1);
  g_pCamera->SetPlrViewRange(40);

	FilmFadeIn();
	
	SetNext("BibSeq2", 1);
}

func BibSeq2()
{
	DialogMessage(pMarc, "Hier muss nun irgendwo das besagte rote Buch liegen...", "BibSeq3", 0, 0, "Dialog18.ogg");
}

local pGuard1;
local pGuard2;

func BibSeq3()
{
	var pObj = CreateObject(KARH, 0, 0, -1);
	pObj->SetPosition(916, 417);
	pObj->SetAction("AimBow");
	pObj->SetColorDw(RGB(200,200));
	pObj->SetPhase(4);
	pObj->SetDir(0);
	pObj->PoffClonk();
	pGuard1 = pObj;

	pObj = CreateObject(KARH, 0, 0, -1);
	pObj->SetPosition(950, 410);
	pObj->SetAction("AimBow");
	pObj->SetColorDw(RGB(200,200));
	pObj->SetPhase(5);
	pObj->SetDir(0);
	pObj->PoffClonk();
	pGuard2 = pObj;

	pFenringMage = CreateObject(MAGE, 0, 0, -1);
	pFenringMage->~MakeFenringMage();
	pFenringMage->SetPosition(840, 420);
	pFenringMage->SetAction("Magic");
	pFenringMage->SetDir(1);
	pFenringMage->PoffClonk();
	
	pMarc->CreateContents(CRBW);
	pMarc->CreateContents(BOTP);
	pMarc->CreateContents(ROCK);
	pMarc->CreateContents(ROCK);
	pMarc->CreateContents(ROCK);
	pMarc->LoadCrossbow(FindContents(CRBW), 1);
	
	SetNext("BibSeq4", 1);
}

func BibSeq4()
{
	DialogMessage(pFenringMage, "Waffe runter, Meuchler! Die Show ist aus!", "BibSeq5", 0, 0, "Dialog19.ogg");
	pFenringMage->SetAction("AimMagic");
	pFenringMage->SetPhase(5);
}

func BibSeq5()
{	
	DialogMessage(pMarc, "Args, Maron. Bei Teiwaz, das wirst du noch bereuen!", "BibSeq5b", 0, 0, "Dialog20.ogg");
	pMarc->SetDir(0);
	pMarc->CreateObject(CRBW);
	pMarc->SetAction("HandsUp");
}

func BibSeq5b()
{ 
  DialogMessage(pFenringMage, "Bewacht alle Ausgänge! Vielleicht ist ja noch ein Komplize unterwegs.", "BibSeq6", 0, 0, "Dialog21.ogg");
}



func BibSeq6()
{
	PosCam(pClonk, 0, 1);
	DialogMessage(pClonk, "Hmm, irgendwas muss los sein...", "BibSeq7", 0, 0, "Dialog22.ogg");
}

func BibSeq7()
{
	pGuard1->RemoveObject();
	pGuard2->RemoveObject();
	pFenringMage->RemoveObject();
	pMarc->RemoveObject();
	StopFilm();
}

public func SignpostReached()
{
	DialogMessage(pClonk, "So jetzt habe ich das Buch, nur wo ist Marc hin? Ich hoffe, es ist nichts passiert.", "OutEnd", "Ende", 0, "Dialog23.ogg");
	return 1;
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