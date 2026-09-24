/*-- Tutorial --*/

#strict 2

// FreeRect(1340,640,12,Global()++)

static pMage;
static pClonk;
static fFilm;

static szHelpName;
static szHelpingText;

static const fTEST = 0;
// 228 577
func Initialize()
{
  CreateObject(SP69, 1184, 92, -1)->SetTextID(1);
  DigCastlesFree();
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial2.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
  return;
}

protected func DigCastlesFree()
{
  var obj, x, y, w, h;
  for(obj in FindObjects(Find_Container(), Find_Func("CastlePartWidth"), Find_InRect(0, 0, 400,  620)))
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
  Message(Format("$Mage01$", RGB(250), szMsg), 0);
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
    ScriptGo(1);
    goto(fTEST+10);
    PlaceSunLight();
    FindObject2(Find_ID(PLDN))->SetOwner(player);
  }
  SetFoW(1, player);
  DoWealth(player, 20);
  SetPlrViewRange(150, GetHiRank(player));
  SetPosition(22, 559, GetHiRank(player));
  // Mit Ki verfeinden
  SetHostility(10, player, 1, 1, 1);
  SetHostility(player, 10, 1, 1, 1);
  SetCommand(GetHiRank(player), "MoveTo",  0, 175, 592);
  SetDir(1, GetHiRank(player));
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
  // Angekommen, wer da?
  ShoutMsg("$Mage02$", pClonk);
}

func Script22()
{
  pMage = CreateObject(MAGE, 228, 577, -1);
  SetColorDw(RGB(200,200,0), pMage);
  SetName("Myxus", pMage);
  SetDir(0, pMage);
  PlacementEffects(pMage->GetX(), pMage->GetY());
  ShoutMsg("$Mage03$", pMage);
}

func PlacementEffects(x, y, color1, color2) 
{
  CastParticles("PxSpark", 50, 100, x, y, 5*8, 5*50, RGB(196, 196, 196), RGB(255, 255, 255));

  for (var i = 0; i < 5; i++)
    DrawParticleLine("PSpark", x-Random(400)+200,0, x,y, 8, 150 + 100 * (Random(3) == 0), RGBa(255,50+Random(100),50+Random(100),50), RGBa(50+Random(100),255,50+Random(100),100));
  
  return(1);
}

func Script23()
{
  ShoutMsg("$Mage04$", pClonk);
}

func Script24()
{
  ShoutMsg("$Mage05$", pMage);
}

func Script25()
{
  ShoutMsg("$Mage06$", pMage);
}

func Script26()
{
  ShoutMsg(["$Mage07$", "$Mage08$"], pMage);
}

func Script27()
{
  ShoutMsg("$Mage09$", pMage);  
}

func Script28()
{
  ShoutMsg(["$Mage10$", "$Mage11$"], pMage);
}

func Script30()
{
  if(!FindObject2(Find_ID(MAGE), Find_PlayerType(C4PT_User))) return goto(29);
  ShoutMsg(["$Mage12$","$Mage13$", "$Mage14$"], pMage);
  CreateObject(STES, 10, 10, -1);
}

func Script31()
{
  ShoutMsg(["$Mage15$", "$Mage16$", "$Mage17$"], pMage);
  CreateObject(STES, 10, 10, -1);
}

func Script32()
{
  ShoutMsg(["$Mage18$", "$Mage19$"], pMage);
}

func Script33()
{
  ShoutMsg(["$Mage20$","$Mage21$"], pMage);
}

func Script34()
{
  ShoutMsg(["$Mage22$","$Mage23$", "$Mage24$", "$Mage25$"], pMage);
}

func Script35()
{
  ShoutMsg("$Mage26$", pClonk);
}

func Script36()
{
  ScriptGo(0);
}

func StaffSMARActivated()
{
  ShoutMsg(["$Mage27$","$Mage28$", "$Mage29$"], pMage);
  goto(100);
}

func Script101()
{
  ShoutMsg(["$Mage30$", "$Mage31$"], pMage);
}

func Script102()
{
  ShoutMsg(["$Mage32$", "$Mage33$"], pMage);
}

func Script103()
{
  ScriptGo(0);
}

func StaffSMERActivated()
{
  ShoutMsg(["$Mage34$","$Mage35$", "$Mage36$"], pMage);
  goto(200);
}

func Script201()
{
  ShoutMsg(["$Mage37$", "$Mage38$"], pMage);
}

func Script202()
{
  ShoutMsg(["$Mage39$", "$Mage40$"], pMage);
}

func Script203()
{
  ScriptGo(0);
}

func StaffSMFRActivated()
{
  ShoutMsg(["$Mage41$","$Mage42$", "$Mage43$"], pMage);
  FreeRect(1340,640,12,40);
  goto(300);
}

func Script301()
{
  ShoutMsg(["$Mage44$", "$Mage45$"], pMage);
}

func Script302()
{
  ShoutMsg(["$Mage46$", "$Mage47$"], pMage);
}

func Script303()
{
  ScriptGo(0);
}

func StaffSMWTActivated()
{
  ShoutMsg(["$Mage48$","$Mage49$", "$Mage50$"], pMage);
  goto(400);
}

func Script401()
{
  ShoutMsg(["$Mage51$", "$Mage52$"], pMage);
}

func Script402()
{
  ShoutMsg(["$Mage53$", "$Mage54$"], pMage);
}

func Script403()
{
  ScriptGo(0);
}

protected func RelaunchPlayer(iPlr)
{
  if(GetPlayerType(iPlr)!=C4PT_User) return;
  var id = MCLK;
  var crew = CreateObject(id,22, 569,iPlr);
  MakeCrewMember(crew,iPlr);
  SetCursor(iPlr, crew);
  DoEnergy(100, crew);
  SetPlrViewRange(150, crew);
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
  AddMenuItem("$Next01$", "ScriptGo(1)", LMM2, obj, 0, obj, 0, 2, 4);
//  SetMenuTextProgress(1, obj);
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

global func FxSpecAquaTimer(pAqua, iNumber)
{
  var pClonk, iEff;
  if(pClonk = FindObject2(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Exclude(pAqua), Find_Distance(50), Sort_Distance()))
  {
    if(iEff = GetEffect("ChangeNSpell", pClonk))
      if(EffectVar(3, pClonk, iEff) == ACLK)
      {
        Message("$Mage55$", pAqua);
        Local(0, pAqua)->SetComDir(COMD_Up);
	FindObject(RCTP)->Activate();
        return -1;
      }
    Message("$Mage56$", pAqua);
  }
}

global func FxSpecDyingClonkTimer(pClonk, iNumber)
{
  var pClonk, iEff, pTest;
  if(pTest = FindObject2(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Exclude(pClonk), Find_Distance(250), Sort_Distance()))
  {
    if(GetEnergy(pClonk)>10)
      {
        Message("$Mage57$", pClonk);
        ObjectSetAction(pClonk, "Walk", 0, 0, 1);
        SetCommand(pClonk, "MoveTo", 0, GetX(pClonk)-35, GetY(pClonk));
        return -1;
      }
    Message("$Mage58$", pClonk);
  }
}

public func MsgSignpost1()  { return "$Mage59$"; }

public func SignpostReached()
  {
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial3.c4s", "$BtnNextRound$", "$BtnNextRoundDesc$");
  }
  
