/*-- Story --*/

#strict 2

func Initialize() { FramesLeft = -1; }

protected func MenuQueryCancel() { return 1; }

local pGuard1;
local pGuard2;
local pMarc;
local pClonk;
local pGallow;

local pKing;
local pMerthen;
local pDavid;

func Start()
{
  DoPlayMusic("The Pyre.ogg");  
	ScheduleCall(this, "DoStart", 1, 1);
}

func DoStart()
{
	// Starte in einer anderen Section
  SaveObjects();
	LoadScenarioSection("Capital", 3);
  // Alles wiederherstellen
  RestoreSectObjs();
	
	StartFilm();

  pClonk = GetHiRank(GetPlayerByIndex(0, C4PT_User));
  if(pClonk) pClonk->SetPosition(876, 362);


	pKing = FindObject(KING);
	pKing->SetCommand(pKing, "Enter", FindObject(THRN));
	
	pMerthen = CreateObject(PLDN, 134, 289+10, 11);
	pMerthen->MakeMerthen();
	
	pDavid = CreateObject(PLDN, 105, 289+10, 11);
	pDavid->MakeDavid();
	
	pMerthen->SetDir(0);
	pDavid->SetDir(0);

  g_pCamera->SetPlrViewRange(200);
	FilmFadeDark();
	PosCam(pKing, 0, 1);

	DialogMessage([BO69, "Buch Harkon, Kapitel 2", "1"], "Während Harkon versucht Marc noch zu retten, findet auf der Burg des Königs eine wichtige Besprechung statt.", "Text0", 0, 0, "Dialog1.ogg");
}

func Text0()
{
	FilmFadeIn();
	SetNext("Text1", 1);
  DoPlayMusic("The fall of Gilead.ogg"); 
}

func Text1()
{
  // Auftrag
  DialogMessage(pKing, "David Heerführer des Orden der Sawelô| und Merthen Anführer der Legion des Teiwaz| die Zeiten stehen schlecht.", "Text2", 0, 0, "Dialog2.ogg");
}

func Text2()
{
  // Auftrag
  DialogMessage(pKing, "Die Kanderiander schlagen unser Heer immer wieder zurück und wir haben Probleme mit den Nachschublieferungen.", "Text3", 0, 0, "Dialog3.ogg");
}

func Text3()
{
  // Auftrag
  DialogMessage(pDavid, "Sollte sich nicht Fenring um die Lieferung der Verpflegung kümmern? In seinen Ländereien leben die meisten Bauern.", "Text4", 0, 0, "Dialog4.ogg");
	PosCam(pDavid);
}

func Text4()
{
  // Auftrag
  DialogMessage(pMerthen, "Wir müssen die Rebellion niederschlagen und jeden töten, auf den der Verdacht fällt diese zu unterstützen!", "Text5", 0, 0, "Dialog5.ogg");
	PosCam(pMerthen);
	pMerthen->JumpUp();
}

func Text5()
{
  // Auftrag
  DialogMessage(pDavid, "Glaubst du etwa das Fenring mit der Rebellion verstrickt ist? Wir sollten unsere Kapazitäten nicht verschwenden um die eigenen Leute abzuschlachten.", "Text6", 0, 0, "Dialog6.ogg");
	PosCam(pDavid);
	pDavid->SetDir(1);
}

func Text6()
{
  // Auftrag
  DialogMessage(pMerthen, "Bist du nicht am Ende ein Verräter und dienst den Rebellen?", "Text7", 0, 0, "Dialog7.ogg");
	PosCam(pMerthen);
	pMerthen->UseTwoHandSword();
	fEvaded = 0;
	ScheduleCall(this, "Text6b", 17);
}

local fEvaded;

func Text6b()
{
	pDavid->SetDir(); pDavid->Jump(); pDavid->SetDir(1);
	SetCommand(pKing, "MoveTo", 0, 31, 242);
	fEvaded = 1;
}

func Text7()
{
	if(!fEvaded)
	{
		Text6b();
		ClearScheduleCall();
		ScheduleCall(this, "Text7", 5);
		return;
	}
  // Auftrag
  DialogMessage(pKing, "Ihr benehmt euch wie Kinder! So werden wir den Krieg niemals gewinnen. David, du siehst nach, was bei Fenring los ist und Merthen, du schaust an die Front, ob dort was falsch läuft mit der Versorgung.", "Text8", 0, 0, "Dialog8.ogg");
	pKing->SetAction("Magic");
	PosCam(pKing);
}

func Text8()
{
	pKing->SetCommand(pKing, "Enter", FindObject(THRN));
	pMerthen->SetCommand(pMerthen, "MoveTo", 0, 509, 350);
	pDavid->SetCommand(pDavid, "MoveTo", 0, 509, 350);
	FilmFadeOut();
	SetNext("EndIntroA", 4);
}

func EndIntroA()
{
	StopFilm();
	// Starte in einer anderen Section
  pClonk = GetHiRank(GetPlayerByIndex(0, C4PT_User));
  pClonk->Hide(1, 1, 1);
  SaveObjects();
	LoadScenarioSection("Map", 3);
  // Alles wiederherstellen
  RestoreSectObjs();

	
	pClonk = GetHiRank(GetPlayerByIndex(0, C4PT_User));
	pClonk->Hide(1, 1, 1);

	StartFilm();
	FilmFadeIn();
	
	pGuard1 = Object(2226);
	pGuard2 = Object(2227);
	pMarc = Object(2228);

	pGuard1->AI_Disable();
	pGuard2->AI_Disable();

	pGuard1->SetPortrait("Armored2");
	pGuard2->SetPortrait("Armored3");
	pGuard1->SetPhysical("CanScale", 1, 2);
	pGuard2->SetPhysical("CanScale", 1, 2);
	pMarc->MakeMarc();

	pClonk->SetPosition(11, 329);

	PosCam(pGuard1, 0, 1);
  g_pCamera->SetPlrViewRange(40);
	SetNext("TextB1", 1);
}

func TextB1()
{
	pGuard1->SetDir(0);
	DialogMessage(pGuard1, "Also, wenn ein Alarm kommt, soll der Meuchler hier sofort getötet werden, lautet der Auftrag von Fenring.", "TextB2", 0, 0, "Dialog9.ogg");
}

func TextB2()
{
	DialogMessage(pGuard2, "Jap. Damit wir kein Risiko eingehen und er nicht mit dem Leben davon kommt, falls ihn jemand retten will.", "TextB3", 0, 0, "Dialog10.ogg");
}

func TextB3()
{
	DialogMessage(pGuard1, "Nagut, jetzt müssen wir nur noch auf den Henker warten, bis wir losgehen können.", "TextB4", 0, 0, "Dialog11.ogg");
}

func TextB4()
{
	DialogMessage(pClonk, "Hmm, das könnte recht schwierig werden, Marc da raus zu holen. Juncus hat mir diesen Rettungstrank gegeben, mit dem man angeblich das Erhängen überleben soll.", "TextB5", 0, 0, "Dialog12.ogg");
}

func TextB5()
{
	pGuard1->SetDir(1);
	DialogMessage(pClonk, "Die Muskeln versteifen sich und werden so hart, dass sie die Wucht der Schlinge abfangen. So dass man keinen Genickbruch erleidet. Müsste ich nur noch an ihn ran kommen und ihm den Trank verabreichen können.", "TextB6", 0, 0, "Dialog13.ogg");
}

func TextB6()
{
	DialogMessage(pClonk, "Vielleicht sollte ich den Henker überwältigen und mich mit seinen Sachen als er ausgeben. Dann wäre ich bei der Hinrichtung als nähester bei Marc...", "EndIntro", 0, 0, "Dialog14.ogg");
}

func EndIntro()
{
	pGuard1->AI_Enable();
	pGuard2->AI_Enable();
	StopFilm();
	StartSzen();
}

func Encounter(pEncounter, fDead)
{
	StartFilm();
	PosCam(pEncounter, 0, 1);

	for(var pClonk in FindObjects(Find_ID(ASAS)))
		pClonk->StopClonkEx();
	if(fDead)
	{
		if(pEncounter == pGuard1)
			DialogMessage(pEncounter, "Verdammt, eine Leiche! Es muss ein Meuchler da sein. Stich das Opfer lieber schnell ab.", "Encounter3", 0, 0, "Dialog15.ogg");
		else
			DialogMessage(pEncounter, "Alarm, ich habe eine Leiche gefunden!", "Encounter2", 0, 0, "Dialog16.ogg");
	}
	else if(pEncounter == pGuard1)
		DialogMessage(pEncounter, "Hey, hier ist ein Meuchelmörder, stich schnell unser Opfer ab!", "Encounter3", 0, 0, "Dialog17.ogg");
	else
		DialogMessage(pEncounter, "Alarm, hier ist ein Meuchelmörder!", "Encounter2", 0, 0, "Dialog18.ogg");
}

func Encounter2()
{
	pGuard1->SetDir(0);
	DialogMessage(pGuard1, "Dann stich schnell unser Opfer ab!", "Encounter3", 0, 0, "Dialog19.ogg");
}

func Encounter3()
{
	pMarc->SetPosition(GetX(pGuard2)+10, GetY(pGuard2));
	pGuard2->FightWith(pMarc);
	pGuard2->SetAction("SwordStrike");
	Schedule("pMarc->Kill()", 4*4);
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
		GetCrew(GetPlayerByIndex(i, C4PT_User))->OpenRelauchMenu("Marc ist gestorben.");
}

func DoStartSzen()
{
	pGuard1 = Object(2226);
	pGuard2 = Object(2227);
	pMarc = Object(2228);
	pMarc->MakeMarc();
	pGallow = Object(2271);

	pGuard1->SetPortrait("Armored2");
	pGuard2->SetPortrait("Armored3");
	pGuard1->SetPhysical("CanScale", 1, 2);
	pGuard2->SetPhysical("CanScale", 1, 2);
	pGuard1->SetPhysical("Jump", 40000, 2);
	pGuard2->SetPhysical("Jump", 40000, 2);
	
	pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
	pClonk->CreateContents(_SPN);
	CallOnCondition("HangmanDisguisedArrived", this, "CheckHarkonNearGuards");

//	CallOnSeen("HangmanSeen", Object(2275), this);
  CallOnCondition("HangmanSeen", this, "NearHangman");

  for(var pObj in FindObjects(Find_ID(SCRL)))
  {
    pObj->RemoveObject();
  }
  
	for(var pObj in FindObjects(Find_Or(Find_ID(KNIG), Find_ID(KARH))))
	{
//		pObj->AI_Enable(100);
		if(pObj != pGuard1 && pObj != pGuard2)
		{
//			pObj->SetPhysical("Walk", 30000, 2);
			// Bis auf den vorne am Turm sind alle anderen "CityGuards"
			if(pObj != Object(2247)) CallOnDeath("CityGuardDead", pObj, this);
		}
		else
		{
			pObj->AI_Enable(60);
			pObj->LocalN("ai_fFindAssassins") = 1;
		}
		pObj->LocalN("ai_idFirstEncounterCB") = ENMY;
	}
	for(var pObj in FindObjects(Find_ID(_MNR)))
  {
    pObj->LocalN("ai_idFirstEncounterCB") = ENMY;
  }
  HideAttackers();
  DoPlayMusic(["The Shining City.ogg", "Ruins.ogg"]);
}

local attackers;
local attacker_count;

func HideAttackers()
{
  if(attacker_count) return;
  attackers = [];
  attacker_count = 0;

  Hide(Object(2278));
  Hide(Object(2143));
  Hide(Object(2279));
  Hide(Object(2142));//Swordknight
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

local iDeadGuards;

func CityGuardDead(pGuard)
{ 
	iDeadGuards++;//Log("%d Guards dead %v",iDeadGuards,pGuard);
	// Wieder mal typisch ausgerechtnet der letzte hatte den Schlüssel!
	if(iDeadGuards == 12)
	{
		SetPosition(GetX(pClonk), GetY(pClonk), Object(2281));
    Object(2281)->Enter(pClonk);
		pClonk->StopClonkEx();
		DialogMessage(pClonk, "Ah, der hatte den Schlüssel dabei!", 0, 0, 0, "Dialog20.ogg");
  }
}

func NearHangman()
{
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    var iPlr = GetPlayerByIndex(i, C4PT_User);
    var pObj = GetCursor(iPlr);
    if(pObj->GetX() > 704 && pObj->GetX() < 737 && pObj->GetY() > 432 && pObj->GetY() < 463)
      return 1;
  }
}

func HangmanSeen()
{
        pClonk->StopClonkEx();
        DialogMessage(pClonk, "Hmm Mist, der Henker hat abgeschlossen. Bestimmt hat aber eine der Wachen einen Zweitschlüssel.", "Nothing", 0, 0, "Dialog21.ogg");
}

func Nothing() {}

func CheckHarkonNearGuards() { if(pClonk->IsDisguised() && ObjectDistance(pClonk, pGuard1) < 50) return 1; }

func HangmanDisguisedArrived()
{
        pGuard1->AI_Disable();
        pGuard2->AI_Disable();
        
        StartFilm();
        PosCam(pClonk, 0, 1);
        pClonk->StopClonkEx();
        DialogMessage(pGuard1, "Da bist du ja endlich, Henkersmann. Wurde aber auch langsam Zeit. Lass uns losgehen.", "TextC1", 0, 0, "Dialog22.ogg");
}

func TextC1()
{
        PosCam(pClonk);
        pGuard2->SetMacroCommand("MoveTo", 0,  1254, 259);//934, 340);
        pGuard1->SetMacroCommand("MoveTo", 0, 1370, 271);//1023, 339);
        pMarc->SetMacroCommand("MoveTo", 0,  1305, 239);//985, 310);
        pClonk->SetMacroCommand("MoveTo", 0,  1290, 241);//974, 310);

/*        AddCommand(pGuard2, "Jump");
        AddCommand(pGuard1, "Jump");
        AddCommand(pMarc,   "Jump");
        AddCommand(pClonk,  "Jump");
        
        AddCommand(pGuard2, "MoveTo", 0, 623, 427);
        AddCommand(pGuard1, "MoveTo", 0, 623, 427);
        AddCommand(pMarc,   "MoveTo", 0, 623, 427);
        AddCommand(pClonk,  "MoveTo", 0, 623, 427);*/

        CallOnCondition("HangmanNear", this, "CheckHarkonNearGallow");
        CallOnCondition("GuardFoundDead", this, "CheckGuardSeesDead");
        SetNext("EnforceHangmanPosition", 50);
        ShowAttackers();
}

func CheckGuardSeesDead()
{
        if(FindObject2(Find_ID(KNIG),Find_NoContainer(), Find_Distance(100, pGuard1->GetX()-GetX(), pGuard1->GetY()-GetY()),
                Find_OCF(OCF_Living), Find_Action("Dead"), Find_Allied(GetOwner(pGuard1))))
                return true;
}

func GuardFoundDead()
{
        pGuard2->StopClonkEx();
        pGuard1->StopClonkEx();
        pMarc->StopClonkEx();
        pClonk->StopClonkEx();
        Encounter(pGuard1, 1);
}

func CheckHarkonNearGallow() { if(ObjectDistance(pClonk, pGallow) < 160) return 1; }

local aCrowd;

func EnforceHangmanPosition()
{
  pGuard2->SetPosition(1254, 259);
  pGuard1->SetPosition(1370, 271);
  pMarc->SetPosition(1305, 239);
  pClonk->SetPosition(1290, 241);
  HangmanNear();
}

func HangmanNear()
{
        aCrowd = [//Object(2276), Object(2277), Object(2145), Object(2144),
        Object(2278), Object(2143), Object(2279), Object(2142),
                                                Object(2278), Object(2143), Object(2279), Object(2142)];
        MessageEx("Da kommen sie!", aCrowd[1]);
        aCrowd[1]->JumpUp();
        for(var pObj in aCrowd)
        {
                pObj->SetPhysical("CanScale", 1, 2);
                SetDirToPermanent(pClonk, pObj);
        }
        SetNext("TextC2", 2);
}

func TextC2()
{
        MessageEx("Endlich kommen sie!", aCrowd[3]);
        aCrowd[3]->JumpUp();
        SetNext("TextC3", 2);
}

func TextC3()
{
        MessageEx("Wir wollen ihn hängen sehen!", aCrowd[6]);
        aCrowd[6]->JumpUp();
        SetNext("TextC4", 2);
}

func TextC4()
{
        MessageEx("Der dreckige Rebell soll sterben!", aCrowd[2]);
        aCrowd[2]->JumpUp();
        SetNext("TextC5", 2);
}

func TextC5()
{
        DialogMessage(pClonk, "Schweigt, niederes Volk!", "TextC6", 0, 0, "Dialog23.ogg");
}

func TextC6()
{
        DialogMessage(pClonk, "Ich werde jetzt die Schlinge anlegen.", "TextC7", 0, 0, "Dialog24.ogg");
        pGallow->MakeReady(pMarc);
}

func TextC7()
{
        DialogMessage(pClonk, "<i>flüsternd</i> Marc, trink das hier.", "TextC8", 0, 0, "Dialog25.ogg");
        var pPoition = FindObject(_SPN);
        pPoition->Exit();
        pPoition->RemoveObject();
}

func TextC8()
{
        DialogMessage(pMarc, "Sawelô sei gepriesen!", "TextC9", 0, 0, "Dialog26.ogg");
}

func TextC9()
{
        DialogMessage(pGuard1, "Werden wir im letzten Moment dann doch noch religiös, Meuchler?", "TextC10", 0, 0, "Dialog27.ogg");
}

func TextC10()
{
        DialogMessage(pClonk, "Genug Gerede. Jetzt wird er gehängt.", "TextC11", 0, 0, "Dialog28.ogg");
}

func TextC11()
{
        pGallow->DoHang(pMarc);
        SetNext("TextC12", 4);
}

func TextC12()
{
        DialogMessage(pGuard2, "So, das war's. Dann geht nach Hause Leute.", "TextC13", 0, 0, "Dialog29.ogg");
        var pChapel = Object(2650);
        for(var pObj in aCrowd)
        {
                RemoveDirToPermanent(pObj);
                SetCommand(pObj, "Enter", pChapel);
                AddCommand(pObj, "Wait", 0, 0, 0, 0, 0, Random(35*2));
        }
}

func TextC13()
{
        var pChapel = Object(2650);
        DialogMessage(pGuard1, "Wir sollen dann auch mal gehen. Hier gibt es nichts mehr zu tun.", "TextC14", 0, 0, "Dialog30.ogg");
        pGuard2->SetMacroCommand("MoveTo", 0, 10, 10);
        pGuard1->SetMacroCommand("MoveTo", 0, 10, 10);
        PosCam(pClonk);
}

func TextC14()
{
        DialogMessage(pClonk, "Oh, Sawelô sei wahrlich gepriesen, das ging ja nochmal gut aus. Jetzt muss ich nur noch bis zur Nacht warten und dann Marc hier wegschaffen.", "TextCend", 0, 0, "Dialog31.ogg");
}

func TextCend()
{
        FilmFadeOut();
        SetNext("End", 4);
}

func End()
{
        GainMissionAccess(g_mission_access);
        SetNextMission(g_next_mission, "&Nächste Mission", "Die nächste Mission starten.");
        GameOver();
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
