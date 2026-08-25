/*-- Tutorial --*/

#strict 2

static pClonk;
static pPriest;
static pWipfSkull;
static pCrystal;
static fFilm;

static fPaladin;

static szHelpName;
static szHelpingText;

static const fTEST = 0;

func Initialize()
{
  pPriest = FindObject(PRST);
  SetName("Paolo", pPriest);
  pWipfSkull = FindObject(PLDN);
  pCrystal = FindObject(CRYS, 1181, 523, -1, -1);

  CreateObject(SP69, 1715, 485, -1)->SetTextID(1);
  DigCastlesFree();
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
  return;
}

protected func DigCastlesFree()
{
  var obj, x, y, w, h;
  for(obj in FindObjects(Find_Container(), Find_Or(Find_Func("CastlePartWidth"), Find_ID(ELEV))))
  {
    x = GetX(obj)+GetDefCoreVal("Offset","DefCore",GetID(obj),0);
    y = GetY(obj)+GetDefCoreVal("Offset","DefCore",GetID(obj),1);
    w = GetDefCoreVal("Width","DefCore",GetID(obj),0);
    h = GetDefCoreVal("Height","DefCore",GetID(obj),0);
    FreeRect(x,y,w,h);
  }
}

global func NewTask(szMsg, szHelp)
{
  Sound("Ding");
  Message(Format("<c %x>Aufgabe:</c> %s", RGB(250), szMsg), 0);
  szHelpName = szMsg;
  szHelpingText = szHelp;
}

global func StartFilm()
{return;
  /*var iCount, pObj, i, iPlr;
  for(i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    iPlr = GetPlayerByIndex(i,C4PT_User);
    iCount = GetCrewCount(iPlr);
    while (iCount--) {
      pObj = GetCrew(iPlr, iCount);
      SetCrewEnabled(0, pObj);
    }
  }
  fFilm = 1;*/
}

global func StopFilm()
{return;
  /*var iCount, pObj, i, iPlr;
  for(i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    iPlr = GetPlayerByIndex(i,C4PT_User);
    iCount = GetCrewCount(iPlr);
    while (iCount--) {
      pObj = GetCrew(iPlr, iCount);
      SetCrewEnabled(1, pObj);
    }
    SetCursor(iPlr, GetHiRank(iPlr));
  }
  fFilm = 0;*/
}

protected func InitializePlayer(int player, int tx, int ty, object pBase, int iTeam)
{
  // Erster Spieler? dann starten
  if(GetPlayerByIndex(0, C4PT_User)==player)
  {
    pClonk = GetHiRank(player);
    CreateContents(FLAG, pClonk);
    DoWealth(player, 150);
    // Segelboot setzen
    SetPosition(18, 747, FindObject2(Find_ID(SLBS)));
    SetPosition(23, 757, pClonk);
    SetDir(1, pClonk);
    SetWind(100);
//    if(!fTEST) fFilm = 1;
    ScriptGo(1);
    goto(fTEST);
  }
  else
  {
    SetPosition(23, 757, GetHiRank(player));
    CreateObject(TACC, 10, 10);
    CreateObject(TKNW, 10, 10);
  }
  // Läuft schon ein "Film"? Dann gleich ohne Crew starten
  var iCount, pObj;
  if(fFilm)
  {
    iCount = GetCrewCount(player);
    while (iCount--) {
      pObj = GetCrew(player, iCount);
      SetCrewEnabled(0, pObj);
    }
  }
  return 1;
}

func Script21()
{
  // Angekommen, Boot anlegen und an Land laufen
  FindObject2(Find_ID(SLBS))->LandOn();
  SetCommand(pClonk, "MoveTo", 0,306,723);
  ShoutMsg("$MsgText1$", pClonk);
}

func Script22()
{
  ShoutMsg("$MsgText2$", pPriest);
}

func Script23()
{
  ShoutMsg(["$MsgText3$","$MsgText3b$"], pPriest);
}

func Script24()
{
  var i;
  for(i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    SetCrewEnabled(1, FindObject2(Find_Owner(GetPlayerByIndex(i, C4PT_User)), Find_ID(KNIG)));
    StartChurchChoose(GetPlayerByIndex(i, C4PT_User));
  }
  ScriptGo(0);
}

func Script25()
{
  ShoutMsg("$MsgText4$", pPriest);
}

func Script26()
{
  ShoutMsg(["$MsgText5$","$MsgText5b$","$MsgText5c$"], pPriest);
}

func Script27()
{
  ShoutMsg(["$MsgText6$","$MsgText6b$","$MsgText6c$"], pPriest);
}

func Script28()
{
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    ShoutMsg("$MsgText7$", pPriest);
  else ShoutMsg("$MsgText7b$", pPriest);
}

func Script29()
{
  ShoutMsg("$MsgText8$", pPriest);
}

func Script30()
{
  ShoutMsg(["$MsgText9$","$MsgText9b$"], pPriest);
  AddCommand(pPriest, "MoveTo", 0, 627, 760);
}

func Script31()
{
  StopFilm();
  Sound("Ding");
  NewTask("$Task1$", "$TastDesc1$");
  goto (100);
}

func Script100()
{
  if(!FindObject2(Find_ID(BHLG))) return goto(90);
  ShoutMsg(["$MsgText10$","$MsgText10b$"], pPriest);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),HLWT);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),ACLD);
  DoHomebaseMaterial(GetPlayerByIndex(0, C4PT_User), CNKT, 1);
  DoHomebaseMaterial(GetPlayerByIndex(0, C4PT_User), ALC_, 1);
}

func Script120()
{
  Sound("Ding");
  NewTask("$Task2$", "$TaskDesc2$");
  goto (150);
}

func Script150()
{
  if(!FindObject2(Find_ID(HLWT))) return goto(140);
  ShoutMsg(["$MsgText11$","$MsgText11b$"], pPriest);
}

func Script151()
{
  ShoutMsg("$MsgText12$", pPriest);
  SetCommand(pPriest, "Call", pPriest, 0, 0, 0, "ContextPray");
  AddCommand(pPriest, "MoveTo", 0, 627, 760);
  fPaladin = 1;
}

func Script152()
{
  Sound("Ding");
  NewTask("$Task3$", "$TaskDesc3$");
  goto (200);
}

func Script200()
{
  if(!(pClonk = FindObject2(Find_ID(PLDN), Find_InRect(940,0,50,LandscapeHeight())))) return goto(199);
  ShoutMsg("$MsgText13$", pClonk);
  SetComDir(COMD_Stop, pClonk);
  SetDir(1, pClonk);
  StartFilm();
}

func Script201()
{
  ShoutMsg("$MsgText14$", pWipfSkull); 
}

func Script202()
{
  ShoutMsg("$MsgText15$", pClonk);
//  pClonk->SetAction("Pray");
}

func Script203()
{
  ShoutMsg("$MsgText16$", pWipfSkull);
  SetCommand(pWipfSkull, "Call", FindObject2(Find_ID(CPT4), Find_AtPoint(1078, 687)), 0, 0, 0, "ControlDownSingle");
  AddCommand(pWipfSkull, "Grab", FindObject2(Find_ID(CPT4), Find_AtPoint(1078, 687)));
}

func Script204()
{
  ShoutMsg(["$MsgText17$","$MsgText17b$"], pWipfSkull);
  AddCommand(pWipfSkull, "MoveTo", 0, 1155, 707);
//  pClonk->SetAction("KneelUp");
}

func Script205()
{
  ShoutMsg(["$MsgText18$","$MsgText18b$"], pWipfSkull);
}

func Script206()
{
  ShoutMsg(["$MsgText19$","$MsgText19b$"], pWipfSkull);
}

func Script207()
{
  ShoutMsg(["$MsgText20$","$MsgText20b$","$MsgText20c$"], pWipfSkull);
}

func Script208()
{
  ShoutMsg("$MsgText21$", pWipfSkull);
}

func Script209()
{
  Sound("Ding");
  NewTask("$Task4$", "$TaskDesc4$");
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),MUSK);
  StopFilm();
  goto (300);
}

func Script300()
{
  if(!FindObject2(Find_ID(MUSK))) return goto(290);
  ShoutMsg("$MsgText22$", pWipfSkull);
}

func Script301()
{
  Sound("Ding");
  NewTask("$Task5$", "$TaskDesc5$");
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),BLTP);
  DoHomebaseMaterial(GetPlayerByIndex(0, C4PT_User), GUNP, 5);
  var pBird = pCrystal->CreateObject(_BRD);
  pBird->SetColorDw(RGB(50,200,50));
  pBird->AddItem(CreateObject(PBLP));
  LocalN("pFix", pBird) = pCrystal;
  pBird->CastParticles("FSpark", 30,100, 0,0, 100, 200, RGBa(128,128,255,0), RGBa(255,255,255,127));
}

func Script310()
{
  if(FindObject2(Find_ID(_BRD))) return goto(305);
  ShoutMsg("$MsgText23$", pWipfSkull);
}

func Script311()
{
  Sound("Ding");
  NewTask("$Task6$", "$TaskDesc6$");
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),PBLP);
  DoHomebaseMaterial(GetPlayerByIndex(0, C4PT_User), GUNP, 5);
  var pBird = pCrystal->CreateObject(_BRD);
  pBird->SetColorDw(RGB(200,60,50));
  pBird->AddItem(CreateObject(SBLP));
  LocalN("pFix", pBird) = pCrystal;
  pBird->CastParticles("FSpark", 30,100, 0,0, 100, 200, RGBa(128,128,255,0), RGBa(255,255,255,127));
}

func Script320()
{
  if(FindObject2(Find_ID(_BRD))) return goto(315);
  ShoutMsg("$MsgText24$", pWipfSkull);
}

func Script321()
{
  Sound("Ding");
  NewTask("$Task7$", "$TaskDesc7$");
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),SBLP);
  DoHomebaseMaterial(GetPlayerByIndex(0, C4PT_User), GUNP, 5);
  var pPlan = CreateObject(LSCR);
  pPlan->SetSpell(TSWD);
  var pBird = pCrystal->CreateObject(_BRD);
  pBird->SetColorDw(RGB(30,60,200));
  pBird->AddItem(pPlan);
  LocalN("pFix", pBird) = pCrystal;
  pBird->CastParticles("FSpark", 30,100, 0,0, 100, 200, RGBa(128,128,255,0), RGBa(255,255,255,127));
}

func Script330()
{
  if(FindObject2(Find_ID(_BRD))) return goto(325);
  ShoutMsg("$MsgText25$", pWipfSkull);
}

func Script331()
{
  Sound("Ding");
  NewTask("$Task8$", "$TaskDesc8$");
}

func Script340()
{
  if(!FindObject2(Find_ID(TSWD))) return goto(335);
  ShoutMsg(["$MsgText26$","$MsgText26b$"], pWipfSkull);
}

func Script341()
{
  Sound("Ding");
  NewTask("$Task8b$", "$TaskDesc8b$");
  var pObj = FindObject2(Find_ID(SKLT));
  pObj->SetAlive(1);
  pObj->DoEnergy(10);
  pObj->SetAI();
  pObj->SetAction("KneelUp");
}

func Script343()
{
  if(FindObject2(Find_ID(SKLT), Find_OCF(OCF_Alive))) return goto(342);
  var pObj = FindObject2(Find_ID(SKLT));
  pObj->SetAlive(1);
  pObj->DoEnergy(10);
  pObj->SetAI();
  pObj->SetAction("KneelUp");
}

func Script345()
{
  if(FindObject2(Find_ID(SKLT), Find_OCF(OCF_Alive))) return goto(344);
  var pObj = FindObject2(Find_ID(SKLT));
  pObj->SetAlive(1);
  pObj->DoEnergy(10);
  pObj->SetAI();
  pObj->SetAction("KneelUp");
  goto(390);
}

func Script400()
{
  if(FindObject2(Find_ID(SKLT))) return goto(390);
  ShoutMsg(["$MsgText27$","$MsgText27a$", "$MsgText27b$"], pWipfSkull);
  RemoveAll(FCWS); RemoveAll(FBRS);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),BAT1);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),PHNX);
}

func Script401()
{
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    ShoutMsg(["$MsgText28$","$MsgText28b$"], pWipfSkull);
  else
    ShoutMsg(["$MsgText28c$","$MsgText28d$"], pWipfSkull);
}

func Script402()
{
  Sound("Ding");
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    NewTask("$Task9$", "$TaskDesc9$");
  else NewTask("$Task9b$", "$TaskDesc9$");
}

func Script420()
{
  if(!FindObject2(Find_Or(Find_ID(BAT1), Find_ID(PHNX)))) return goto(415);
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    ShoutMsg(["$MsgText29$","$MsgText29b$"], pWipfSkull);
  else
    ShoutMsg(["$MsgText29c$","$MsgText29d$"], pWipfSkull);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),BTRL);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),BGNT);
}

func Script421()
{
  Sound("Ding");
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    NewTask("$Task10$", "$TaskDesc9$");
  else NewTask("$Task10b$", "$TaskDesc9$");
}

func Script440()
{
  if(!FindObject2(Find_Or(Find_ID(BTRL), Find_ID(BGNT)))) return goto(435);
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    ShoutMsg(["$MsgText30$","$MsgText30b$"], pWipfSkull);
  else
    ShoutMsg(["$MsgText30c$","$MsgText30d$"], pWipfSkull);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),SARN);
  SetPlrKnowledge(GetPlayerByIndex(0, C4PT_User),GRFN);
}

func Script441()
{
  Sound("Ding");
  if(MMGetChurch(GetOwner(pClonk))==scBloodchurch)
    NewTask("$Task11$", "$TaskDesc9$");
  else NewTask("$Task11b$", "$TaskDesc9$");
}

func Script450()
{
  if(!FindObject2(Find_Or(Find_ID(GRFN), Find_ID(SARN)))) return goto(445);
  ShoutMsg(["$MsgText31$", "$MsgText31b$"], pWipfSkull);
  NewTask("$Task12$", "$TaskDesc12$");
}

func Script460()
{
  if(!FindObject2(Find_InRect(1657, 414, 140, 101), Find_ID(PLDN))) return goto(455);
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial2.c4s", "$BtnNextRound$", "$BtnNextRoundDesc$");
  GameOver();
}

public func SignpostReached()
  {
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial2.c4s", "$BtnNextRound$", "$BtnNextRoundDesc$");
  }
  

func OnChurchChoose(iPlr)
{
//  if(!fTEST) SetCrewEnabled(0, GetHiRank(iPlr));
  if(iPlr == GetPlayerByIndex(0, C4PT_User)) ScriptGo(1);
}

protected func RelaunchPlayer(iPlr)
{
  if(GetPlayerType(iPlr)!=C4PT_User) return;
  var id = KNIG;
  if(fPaladin) id = PLDN;
  var crew = CreateObject(id,23, 757,iPlr);
  MakeCrewMember(crew,iPlr);
  SetCursor(iPlr, crew);
  DoEnergy(100, crew);
}

global func ShoutMsg(sMessage, pSpeaker, szPortrait, iExcludePlayer, fPermanent)
{
  var id = GetID(pSpeaker);
  // Portrait
  if (!szPortrait) if (!(szPortrait = pSpeaker->~GetMsgPortrait())) szPortrait = "1";
  // Portrait ermitteln
  var c,i,p2="";
  if (GetChar(szPortrait) == GetChar("@"))
  {
    // @Portrait: Aus der Dialog Definition holen
    while (c=GetChar(szPortrait, ++i)) p2 = Format("%s%c", p2, c);
    szPortrait = Format("Portrait:%i::%x::%s", DG69, GetColorDw(pSpeaker), p2);
  }
  else
  {
    // Manche Clonks haben kein eignes Portrait und verwenden Standartportraits
    if(id==ACLK || id==_ACK) id = CLNK;
    if(id==WDSK) id = MAGE;
    // Ansonsten direkt vom Sprecher
    szPortrait = Format("Portrait:%i::%x::%s", id, GetColorDw(pSpeaker), szPortrait);
  }
  // Farbe ermitteln
  var dwClr = pSpeaker->~GetDlgMsgColor();
  if (!dwClr) dwClr = GetColorDw(pSpeaker);
  if (!dwClr) dwClr = 255;
  // Permanent @
  var szPermanent = "";
  if(fPermanent) szPermanent = "@";
  // Message ausgeben
  var obj = GetCursor(GetPlayerByIndex(0, C4PT_User));
  SetComDir(COMD_Stop, obj);
  CreateMenu(GetID(pSpeaker),obj,0,0,Format("<c %x>%s:</c>", dwClr, GetName(pSpeaker)),0,C4MN_Style_Dialog);
  AddMenuItem(szPortrait,0,NONE,obj,0,0,0,5);
  var szMsg;
  if(GetType(sMessage)==C4V_String)
    AddMenuItem(sMessage,0,NONE,obj);
  else for(szMsg in sMessage) AddMenuItem(szMsg,0,NONE,obj);
  AddMenuItem("$TxtNext$", "ScriptGo(1)", LMM2, obj, 0, obj, 0, 2, 4);
  ScriptGo(0);
  iExcludePlayer = GetPlayerByIndex(0, C4PT_User)+1;
  var sText = "";
  if(GetType(sMessage)==C4V_String)
    sText = sMessage;
  else for(szMsg in sMessage) sText = Format("%s|%s", sText, szMsg);
  if(iExcludePlayer)
  {
    i = GetPlayerCount(C4PT_User);
    while(i--)
      if(GetPlayerByIndex(i,C4PT_User)!=iExcludePlayer-1) CustomMessage(Format("%s<c %x>%s:</c> %s", szPermanent, dwClr, GetName(pSpeaker), sText), 0, GetPlayerByIndex(i,C4PT_User), 90,70, 0xffffff, MD69, szPortrait, 0);
  }
  else CustomMessage(Format("%s<c %x>%s:</c> %s", szPermanent, dwClr, GetName(pSpeaker), sText), 0, -1, 90,70, 0xffffff, MD69, szPortrait, 0);
}

func MenuQueryCancel()
{
  ScriptGo(1);
}

/* Die Wegweisertexte */

public func MsgSignpost1()  { return "$TxtSign$"; }

global func FxNoBurningEffect(szNewEffect, iEffectTarget, iEffectNumber, iNewEffectNumber, var1, var2, var3)
  {
  // Feuer abblocken
  if (WildcardMatch(szNewEffect, "*Fire*")) return -1;
  // Alles andere ist OK
  return;
  }

protected func FxNoBurningEffect(string szNewEffect)
  {
  if(szNewEffect == "NoBurning") return -1; // Einer reicht
  }
