/* Wiederbelebung */

#strict 2

#appendto CLNK

static g_fIntroSkiped;

global func GoalMessageJuncus(pClonk, szText, szText2)
{
	var szPortrait = Format("Portrait:%i::%x::%s", _PRT, RGB(0,255,255), "Juncus1");
	CreateMenu(WIPF,pClonk,this,0,"Juncus:",0,C4MN_Style_Dialog);
  AddMenuItem(szPortrait,0,NONE,pClonk,0,0,0,5);
  AddMenuItem(szText,0,NONE,pClonk);
	if(szText2)
		AddMenuItem(szText2,0,NONE,pClonk);
  AddMenuItem("Starten", "Nothing", LMM2, pClonk, 0, 0, 0, 2, 4);
}

global func Nothing() { return; }

global func DoInitPlayer(int player)
{
	var index = 0;
	while(GetPlayerByIndex(index, C4PT_User) != player && index < GetPlayerCount(C4PT_User)) index++;
	var pClonk = GetHiRank(player);
	pClonk->MakeHarkon(index+1);
	SetFoW(1, player);
	SetPlrViewRange(0, pClonk);
	pClonk->SetRelaunch();
//	pClonk->SetPlrViewRange(150);

  // Mit Ki verfeinden
	SetHostility(10, player, 1, 1, 1);
//	SetHostility(player, 10, 1, 1, 1);
	SetHostility(12, player, 1, 1, 1);
//	SetHostility(player, 12, 1, 1, 1);
	SetHostility(10, 12, 1, 1, 1);
	
	// Storyobject erzeugen
	if(!FindObject(_STY))
	{
		CreateObject(_STY)->~Start();
	}
}

func MakeHarkon(g_ProtagonistIndex)
{
	if(g_ProtagonistIndex==2)
		return MakeFred();
	if(g_ProtagonistIndex==3)
		return MakeMalvin();
	if(g_ProtagonistIndex>3)
		return;
	if(g_chapter == 1)
	{
    LocalN("szGraphicsName") = "Farmer";
		SetGraphics("Farmer");
		SetPortrait("Farmer1", this, _PRT);
	}
	if(g_chapter == 2)
	{
    LocalN("szGraphicsName") = "Adept";
		SetGraphics("Adept");
		SetPortrait("Adept1", this, _PRT);
	}
	SetName("Harkon");
}

func MakeOlrog()
{
	SetColorDw(RGB(0,0,255));
	SetName("Olrog");
	SetPortrait("Olrog1", this, _PRT);
}

func MakeMarc()
{
	SetColorDw(RGB(61,100,61));
	SetName("Marc");
  LocalN("szGraphicsName") = "Marc";
	SetGraphics("Marc");
	SetPortrait("Marc1", this, _PRT);
}

func MakeFred()
{
	SetColorDw(RGB(255,0,0));
	SetName("Fred");
  LocalN("szGraphicsName") = "Fred";
	SetGraphics("Fred");
	SetPortrait("Fred1", this, _PRT);
}

func MakeMalvin()
{
	SetColorDw(RGB(255,255,0));
	SetName("Malvin");
  LocalN("szGraphicsName") = "Malvin";
	SetGraphics("Malvin");
	SetPortrait("Malvin1", this, _PRT);
}
func MakeJuncus()
{
	SetColorDw(RGB(0,255,255));
	SetName("Juncus");
	SetPortrait("Juncus1", this, _PRT);
}
func MakeCra()
{
	SetColorDw(RGB(255,255,0));
	SetName("Cra");
	SetPortrait("Cra1", this, _PRT);
}
func MakeFenring()
{
	SetColorDw(RGB(61,85,61));
	SetGraphics("Fenring", this, _CLK);
	SetName("Fenring");
	SetPortrait("Fenring1", this, _PRT);
}
func MakeFenringMage()
{
	SetColorDw(RGB(200,30,10));
	SetName("Maron");
}
func MakeMerthen()
{
	SetGraphics("ShieldDark", this, GetID(), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics);
	SetGraphics("ArmoredDark");
	LocalN("fArmored") = ARMR;
	LocalN("idShield") = SHIE;
	SetColorDw(RGB(255,155,0));
	SetPortrait("ArmoredDark2");
	SetName("Merthen");
}
func MakeDavid()
{
	SetGraphics("Shield", this, GetID(), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics);
	SetGraphics("Armored");
	LocalN("fArmored") = ARMR;
	LocalN("idShield") = SHIE;
	SetColorDw(RGB(255));
	SetPortrait("Armored3");
	SetName("David");
}

func GetPortrait (object pObj, bool fGetID, bool fGetPermanent)
{
	var result = _inherited(pObj, fGetID, fGetPermanent);
	if(fGetID)
	{
		if(idPortrait) return idPortrait;
		if(result) return result;
		return GetID();
	}
	if(szPortrait) return szPortrait;
	if(result) return result;
	return "1";
}

local szPortrait;
local idPortrait;

func SetPortrait (string szNewPortrait, object pTarget, id idSrcDef, bool fPermanent, bool fCopyGfx)
{
	szPortrait = szNewPortrait;
	idPortrait = idSrcDef;
	return _inherited(szNewPortrait, pTarget, idSrcDef, fPermanent, fCopyGfx);
}

//C4SECT_SaveLandscape, C4SECT_SaveObjects und C4SECT_KeepEffects

global func StartSzen()
{
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		GetCursor(GetPlayerByIndex(i,C4PT_User))->StopClonkEx();
		GameCall("PlayerStart", GetPlayerByIndex(i,C4PT_User));
	}
	GameCall("DoStartSzen", GetPlayerByIndex(i,C4PT_User));

	// Spielziel erzeugen
	if(!FindObject(SCRG)) CreateObject(SCRG);
	
	// Neu Initialisierung
	var story = FindObject(_STY);
	if(GetID() == _STY) story = this;
	if(!story) story = CreateObject(_STY);
	story->~DoStartSzen();

	// Gemeinschaftskonto
	if(!FindObject(TACC) && GetPlayerCount(C4PT_User) > 1) CreateObject(TACC);
}

public func ContextReset(pCaller)
{
  [Neu starten|Image=_SAV:2]
  ResetGame();
  return 1;
}

func Saved() { return g_GameSaved; }

public func ContextLoad(pCaller)
{
  [Laden|Image=_SAV:1|Condition=Saved]
  ReloadGame();
  return 1;
}

public func ContextSave(pCaller) 
{
  [Speichern|Image=_SAV]
  SaveGame();
  return 1;
}

static g_szSectionName;

global func LoadScenarioSection(szName, iFlags)
{
	g_szSectionName = szName;
	return _inherited(szName, iFlags, ...);
}

global func GetScenarioSection()
{
	if(!g_szSectionName) return "Main";
	return g_szSectionName;
}

public func SetRelaunch()
{
  if (GetEffect("IntRelaunch", this)) return;
  return AddEffect("IntRelaunch", this, 200, 0, this);
}

protected func FxIntRelaunchEffect(string szNewEffect)
  {
  if(szNewEffect == "IntRelaunch") return -1;
  if(szNewEffect == "ReincarnationPSpell") Local(10)=1;
  }

protected func FxIntRelaunchStop(object pTarget, int iEffectNumber, int iReason, int fTmp)
  {
  // Temporäres entfernen: nichts machen
  if(fTmp) return;
  // Noch am Leben? Nichts machen
  if(GetAlive(pTarget)) return -1;
  // Nur wenn noch Besitzer da ist
  if(GetOwner(pTarget)<0) return 1;
  // Nicht wenn es eine Kopie ist
  if(GetEffect("ReplicationSpell", pTarget)) return 1;
  if(GetEffect("AI", pTarget)) return 1;
  if(GetPlayerType(GetOwner(pTarget)) == C4PT_Script) return 1;

  // Wenn er entfernt wurde, neu erzeugen
  if(iReason == 3 || GetY(pTarget)>LandscapeHeight())
  {
    var pOld = pTarget;
    var pSymbol = CreateObject(CXCP,  GetX(pTarget)-GetX(), LandscapeHeight()-GetY()-10, GetOwner());
    pTarget = CreateObject(GetID(pTarget), GetX(pTarget)-GetX(), LandscapeHeight()-GetY()-10, GetOwner());
    Enter(pSymbol, pTarget);
//    MakeCrewMember(pTarget, GetOwner(pTarget));
    GrabObjectInfo(this, pTarget);
    if(!GetCursor(GetOwner())) SetCursor(GetOwner(), pTarget);
    SetPlrViewRange(150, pTarget);
    while(Contents(0,pOld)) Enter(pTarget,Contents(0,pOld));
    // Attachte Objekte sichern (Schilde, Amulette, ect...)
    for(var obj in FindObjects(Find_ActionTarget(pOld)))
    {
      // Nur wirklich attachte Objekte (z.B. keine kämpfenden)
      if(GetProcedure(obj)=="ATTACH")
        obj->~SetActionTagets(pTarget);
    }
    // Spawnkoordinaten übernehmen
    for(var i = 0; i < 8; i++)
      pTarget->Local(10+i) = Local(10+i, pOld);
    ScheduleCall(pTarget, "OpenRelauchMenu", 1, 1, "Du bist gestorben.");
  }
  else
  {
    // Wenn nicht tot, nichts machen
//    if(GetAlive(pTarget)) return -1;
    // Wiederbeleben
    DoEnergy(-GetEnergy(pTarget), pTarget);
    Extinguish(pTarget);
    SetAlive(1, pTarget);
    SetAction("Dead", pTarget);
    DeathAnnounce ();
    Sound("Die");
    pTarget->OpenRelauchMenu("Du bist gestorben.");
		Schedule("SetAlive(0)",1);
  }
//  pTarget->~UnHide();
  
  return -1;
}

local aRespawnMsg;

public func OpenRelauchMenu(szMessage)
{
	if(szMessage)
	{
		if(!aRespawnMsg) aRespawnMsg = [szMessage];
		else
		{
			aRespawnMsg[GetLength(aRespawnMsg)] = szMessage;
			CloseMenu();
		}
	}
  var pTarget = this;
	var pPicture = this;
	if(g_pCamera) pTarget = g_pCamera;
  var szPortrait = Format("Portrait:%i::%x::%s", pPicture->GetPortrait(0, 1), GetColorDw(pTarget), pPicture->GetPortrait(0, 0));
  CreateMenu(WIPF,pTarget,this,0,Format("<c %x>%s:</c>", pPicture->GetColorDw(), pPicture->GetName()),0,C4MN_Style_Dialog);
  AddMenuItem(szPortrait,0,NONE,pTarget,0,0,0,5);
	for(var text in aRespawnMsg)
		AddMenuItem(text,0,NONE,pTarget);
	if(g_GameSaved)
  {
		AddMenuItem("Laden", "ReloadGame()", LMM2, pTarget, 0, 0, 0, 2, 4);
    AddMenuItem("Neustarten", "OpenRelauchMenu2()", LMM2, pTarget, 0, 0, 0, 2, 4);
  }
  else
    AddMenuItem("Neustarten", "ResetGame()", LMM2, pTarget, 0, 0, 0, 2, 4);
//  AddMenuItem("Beenden", "StopGame", MCMX, pTarget);
	if(szMessage)
		AddEffect("KeepMenuOpen", this, 1, 1, this);
}

public func OpenRelauchMenu2(szMessage)
{
  if(szMessage)
  {
    if(!aRespawnMsg) aRespawnMsg = [szMessage];
    else
    {
      aRespawnMsg[GetLength(aRespawnMsg)] = szMessage;
      CloseMenu();
    }
  }
  var pTarget = this;
  var pPicture = this;
  if(g_pCamera) pTarget = g_pCamera;
  var szPortrait = Format("Portrait:%i::%x::%s", pPicture->GetPortrait(0, 1), GetColorDw(pTarget), pPicture->GetPortrait(0, 0));
  CreateMenu(WIPF,pTarget,this,0,Format("<c %x>%s:</c>", pPicture->GetColorDw(), pPicture->GetName()),0,C4MN_Style_Dialog);
  AddMenuItem(szPortrait,0,NONE,pTarget,0,0,0,5);
//  for(var text in aRespawnMsg)
//    AddMenuItem(text,0,NONE,pTarget);
  AddMenuItem("Willst du wirklich neustarten?",0,NONE,pTarget);
  AddMenuItem("Ja", "ResetGame()", LMM2, pTarget, 0, 0, 0, 2, 4);
  AddMenuItem("Nein", "OpenRelauchMenu()", LMM2, pTarget, 0, 0, 0, 2, 4);
  SelectMenuItem(3);
//  AddMenuItem("Beenden", "StopGame", MCMX, pTarget);
  if(szMessage)
    AddEffect("KeepMenuOpen", this, 1, 1, this);
}

func FxKeepMenuOpenTimer()
{
	if(!GetMenu())
		OpenRelauchMenu();
}

protected func MenuQueryCancel()
{
 // Die Relaunchmessage verschwindet nach dem Schließen des Relaunchmenüs
 if (GetMenu() == WIPF) return(1);
}

func RestartGame()
{
	ReloadGame();
}

func StopGame()
{
  EliminatePlayer(GetOwner());
}

func FxIntLaunchStop(pTarget)
{
  DoLaunch(pTarget);
}

func DoLaunch(pTarget)
{
  SetRelaunch();
  CloseMenu();
  aRespawnMsg = 0;
//  SetPlrViewRange(150);
  ObjectSetAction(this, "Walk", 0, 0, 1);
  Message(" ", this);
  if(Local(12, pTarget))
    DoEnergy(Local(12, pTarget)-GetEnergy());
  else DoEnergy(100);

  if(Local(14))
  {
   SetAction(Local(14), Object(Local(20)));
   SetPhase(Local(15));
   SetDir(Local(16));
  }
  if(Local(17))
    Enter(Object(Local(17)));
  SetComDir(COMD_Stop);

  if(!GetEffect("DoubleDamage", pTarget)) AddEffect("DoubleDamage", pTarget, 1,0,pTarget);

	if(Local(19))
		this->~SetDisguised(Local(19));
	else
		this->~SetNotDisguised();
	
	if(Local(18))
		this->~SetHideValue(Local(18));

	if(Local(21))
	{
		Object(Local(21)[0])->SetAction("GettingCarried", this);
		this->~UnHide();
		if(!GetEffect("CarryControlCorps", this))
			AddEffect("CarryControlCorps", this, 1, 1, this, 0, Object(Local(21)[0])->Contents(), Object(Local(21)[0]));
		var iEff = GetEffect("CarryControlCorps", this);
		EffectVar(0, this, iEff) = Local(21)[0];
		EffectVar(1, this, iEff) = Local(21)[1];
		EffectVar(2, this, iEff) = Local(21)[2];
		SetGraphics(0, this,0, 11, GFXOV_MODE_Object, 0, 0, Object(EffectVar(1, this, iEff)));
	}
	else
		RemoveEffect("CarryControlCorps", this);
//	Log("%v", Local(22));
	if(Object(Local(22)) != GetObjectLayer())
	{//Log("%v", Local(22));
		if(GetObjectLayer()) GetObjectLayer()->~RemObject(this);
		if(Object(Local(22))) Object(Local(22))->~AddObject(this);
	}
	if(Local(23))
		LocalN("pQuestStorage") = Object(Local(23));
	
  DoWealth(GetOwner(), Local(13)-GetWealth(GetOwner()));

  Extinguish(pTarget);
  // An die Stelle setzen
  SetPosition(pTarget->Local(10), pTarget->Local(11), pTarget);
  SetXDir(0, pTarget);
  SetYDir(0, pTarget);
  
  var pView = FindObject2(Find_ID(_VIW), Find_ActionTarget(pTarget));
  if(!pView)
  {
    pView = CreateObject(_VIW, 0, 0, GetOwner(pTarget));
    pView->SetAction("Attach", pTarget);
    pView->SetCategory(1);
    pView->SetPlrViewRange(50);
    pTarget->SetPlrViewRange(0);
  }
  pView->DoAttach();
}
//SetObjDrawTransform(-800, 0, 1000, 0, 1000, 1000, this, 1)
global func ResetGame()
{
	StopFilm();
  SaveObjects(0, 1);
	g_GameSaved = 0;
	for(var obj in FindObjects(Find_ID(ASAS), Find_PlayerType(C4PT_User)))
		if(obj->GetObjectLayer()) obj->GetObjectLayer()->RemObject(obj);
	for(var obj in FindObjects(Find_InRect(-GetX(),-GetY(),LandscapeWidth(),LandscapeHeight())))
		if(obj) obj->RemoveObject();
  // Auf in die nächste Section! 
  LoadScenarioSection("Reload", 3);
	for(var obj in FindObjects(Find_InRect(-GetX(),-GetY(),LandscapeWidth(),LandscapeHeight())))
		SaveSectObj(obj);
//SaveMap();
  // Auf in die nächste Section! 
  LoadScenarioSection("Map", 0);
//	for(var obj in FindObjects(Find_InRect(-GetX(),-GetY(),LandscapeWidth(),LandscapeHeight())))
//		if(obj) obj->RemoveObject();
//	LoadMap();
//	DrawMaterialQuad("Tunnel", 0,0, LandscapeWidth()-1,0, LandscapeWidth()-1,LandscapeHeight()-1, 0,LandscapeHeight()-1);
	RedrawMap();
  // Alles wiederherstellen
  RestoreSectObjs(!g_GameSaved);
	g_foundGhosts = g_foundGhostsSaved;
  if(!g_GameSaved)
  {
    GameCall("Initialize");
  }
}
global func ReloadGame()
{
	StopFilm();
  SaveObjects();
  // Auf in die nächste Section! 
  LoadScenarioSection("Main", 0);
  // Auf in die nächste Section! 
  LoadScenarioSection("Map", 0);
  // Alles wiederherstellen
  RestoreSectObjs(!g_GameSaved);
	g_foundGhosts = g_foundGhostsSaved;
  if(!g_GameSaved)
  {
    GameCall("Initialize");
  }
}

static g_GameSaved;

global func SaveGame()
{
	g_foundGhostsSaved = g_foundGhosts;
  SaveObjects(1);
  // Auf in die nächste Section! 
  LoadScenarioSection("Main", 3);
  // Auf in die nächste Section! 
  LoadScenarioSection("Map", 0);
  g_GameSaved = 1;
  // Alles wiederherstellen
  RestoreSectObjs();
}

global func CallOnRestore()
{
	for(var obj in FindObjects(Find_InRect(-GetX(),-GetY(),LandscapeWidth(),LandscapeHeight())))
		obj->~OnRestore();
	GameCall("OnRestore");
}

global func SaveObjects(fAdjustRespawn, fReset)
{
	for(var obj in FindObjects(Find_InRect(-GetX(),-GetY(),LandscapeWidth(),LandscapeHeight())))
		obj->~OnSave();
	GameCall("OnSave");
	
  // Spieler sichern
  var iCount, obj, container;
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    iCount = GetCrewCount(GetPlayerByIndex(i,C4PT_User));
    while (iCount--) {
      obj = GetCrew(GetPlayerByIndex(i,C4PT_User), iCount);
			RemoveEffect("KeepMenuOpen", obj);
			if(!obj->GetAlive())
				obj->SetAlive(1);
      if( fAdjustRespawn && !fReset)
      {
        Local(10, obj) = GetX(obj);
        Local(11, obj) = GetY(obj);
				Local(12, obj) = GetEnergy(obj);
				Local(13, obj) = GetWealth(GetOwner(obj));
				Local(14, obj) = GetAction(obj);
				Local(15, obj) = GetPhase(obj);
				Local(16, obj) = GetDir(obj);
				Local(20, obj) = ObjectNumber(obj->GetActionTarget());
				Local(18, obj) = obj->~GetHideValue();
				Local(19, obj) = obj->~IsDisguised();
				Local(22, obj) = ObjectNumber(obj->GetObjectLayer());
				if(LocalN("pQuestStorage", obj))
					Local(23, obj) = ObjectNumber(LocalN("pQuestStorage", obj));
				else
					Local(23, obj) = 0;
				if(obj->GetObjectLayer()) obj->GetObjectLayer()->~RemObject(obj);
				if(GetEffect("CarryControlCorps", obj))
				{
					var iEff = GetEffect("CarryControlCorps", obj);
					Local(21, obj) = [EffectVar(0, obj, iEff), EffectVar(1, obj, iEff), EffectVar(2, obj, iEff)];
				}
				else
					Local(21, obj) = 0;
				if(Contained(obj))
					Local(17, obj) = ObjectNumber(Contained(obj));
				else Local(17, obj) = 0;
				container = obj->CreateObject(CXCN, 0, 0, -1);
				while(Contents(0, obj)) Enter(container, Contents(, obj));
				SetName(GetName(obj), container);
      }
      else
			{
				if(fReset)
				{
					Local(12, obj) = 0;//obj->GetPhysical("Energy")/1000;
					Local(13, obj) = 0;
					Local(14, obj) = "Walk";
					Local(15, obj) = 0;
					Local(16, obj) = 0;
					Local(20, obj) = 0;
					Local(18, obj) = 0;
					Local(19, obj) = 0;
					Local(22, obj) = 0;
					Local(23, obj) = 0;
					Local(21, obj) = 0;
					Local(17, obj) = 0;
          if(obj->GetObjectLayer()) obj->GetObjectLayer()->~RemObject(obj);
          if(GetEffect("CarryControlCorps", obj)) RemoveEffect("CarryControlCorps", obj);
				}
				if(Contained(obj)) obj->Exit();
				obj->~UnHide();
				while(Contents(0, obj)) Exit(Contents(, obj));
			}
      Exit(obj);
//      ObjectSetAction(obj, "Walk");
      SaveSectObj(obj);
      SetPosition(100, 100, obj);
    }
/*		for(var pMusic in FindObjects(Find_ID(_MUS), Find_Owner(GetPlayerByIndex(i,C4PT_User))))
		{//Log("save %v", pMusic);
			pMusic->SaveSectObj();
			pMusic->SetPosition(100,100);
		}*/
  }
  if(this) SaveSectObj(this);
}

static g_iSaveSectIndex;

global func SaveSectObj(object obj)
  {
  // Kein Objekt?
  if(!obj) return -2;
  if(GetID(obj) == WCHR) return -3; // Bleibt da!
	if(GetID(obj) == _CYH) return -4;
  // Hatten wir das Objekt schon? (Endlosrekursionen sollen vermieden werden)
  var pTest, i;
  while(pTest = Global(20+(i++)*3)) if(pTest == obj) return -1;
  // Inhalt sichern
  var o2,i=obj->ContentsCount(); while (o2=obj->Contents(--i)) SaveSectObj(o2);
  // Attachte Objekte sichern (Schilde, Amulette, ect...)
  o2 = 0;
  for(o2 in FindObjects(Find_ActionTarget(obj)))
  {
    // Nur wirklich attachte Objekte (z.B. keine kämpfenden)
    if(GetProcedure(o2)=="ATTACH")
      SaveSectObj(o2);
  }
  // Objekt sichern
	Global(20+g_iSaveSectIndex) = [];
	Global(20+g_iSaveSectIndex)[1] = GetX(obj);
  Global(20+g_iSaveSectIndex)[2] = GetY(obj);
  (Global(20+(g_iSaveSectIndex++))[0] = obj)->SetObjectStatus(2);
  }

global func RestoreSectObjs(fReInitPlayer)
  {
	for(var obj in FindObjects(Find_InRect(-GetX(),-GetY(),LandscapeWidth(),LandscapeHeight())))
		obj->~OnRestore();
	GameCall("OnRestore");

  var obj;
  // Alles gespeicherte wieder herstellen
	var i = g_iSaveSectIndex;
  if (g_iSaveSectIndex)
	{
    while (i--)
      if (obj=Global(20+i)[0])
        obj->SetObjectStatus(1);
		i = g_iSaveSectIndex;
		while (i--)
		{
      if (obj=Global(20+i)[0])
			{
				SetPosition(Global(20+i)[1], Global(20+i)[2], obj);
				Global(20+i)=0;
			}
		}
		g_iSaveSectIndex=0;
	}

  // Spielerinventar wiederherstellen
  var iCount, obj, container;
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
		var iPlr = GetPlayerByIndex(i,C4PT_User);
		ClearLastPlrCom(iPlr);
    iCount = GetCrewCount(iPlr);
    while (iCount--) {
      obj = GetCrew(GetPlayerByIndex(i,C4PT_User), iCount);
      container = FindCrewByName(CXCN, GetName(obj));
      if(container)
			{
				while(Contents(0, container)) Enter(obj, Contents(, container));
				RemoveObject(container);
			}
			obj->DoLaunch(obj);
    }
//    if(fReInitPlayer) GameCall("PlayerStart", GetPlayerByIndex(i,C4PT_User));
  }
  if(fReInitPlayer) StartSzen();
/*  for(var pMusic in FindObjects(Find_ID(_MUS), Find_Owner(GetPlayerByIndex(i,C4PT_User))))
    {Log("save %v %v", pMusic, GetCursor(pMusic->GetOwner()));
      var pClonk = GetCursor(pMusic->GetOwner());
      pMusic->SetPosition(pClonk->GetX(),pClonk->GetY());
    }*/
  }
