/*-- Tutorial --*/

#strict 2

// FreeRect(1340,640,12,Global()++)

static pTeacher;
static pClonk;
static fFilm;

static szHelpName;
static szHelpingText;

static fCrossbow;
static fKnifes;

static const fTEST = 0;
// 228 577
func Initialize()
{
  pTeacher = FindObject2(Find_ID(ASAS));
  SetName("Theo", pTeacher);
  DigCastlesFree();
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial3.c4s", "$BtnRepeatRound$", "$BtnRepeatRoundDesc$");
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
  Message(Format("$Assassin01$", RGB(250), szMsg), 0);
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
  }
  SetFoW(1, player);
  SetPlrViewRange(150, GetHiRank(player));
  SetPosition(67, 779, GetHiRank(player));
  if(fCrossbow)
  {
    CreateContents(HOOK, GetHiRank(player));
    CreateContents(BOTP, GetHiRank(player));
    CreateContents(NBTP, GetHiRank(player));
    CreateContents(CRBW, GetHiRank(player));
  }
  if(fKnifes) CreateContents(KNFP, GetHiRank(player));
  // Mit Ki verfeinden
  SetHostility(10, player, 1, 1, 1);
  SetHostility(player, 10, 1, 1, 1);
  SetCommand(GetHiRank(player), "MoveTo",  0, 297, 828);
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
  ShoutMsg("$Assassin02$", pClonk);
}

func Script22()
{
  SetCommand(pTeacher, "MoveTo",  0, 362, 766);
  ShoutMsg("$Assassin03$", pTeacher);
}

func Script23()
{
  ShoutMsg("$Assassin04$", pClonk);
}

func Script24()
{
  ShoutMsg("$Assassin05$", pTeacher);
}

func Script25()
{
  ShoutMsg("$Assassin06$", pClonk);
}

func Script26()
{
  ShoutMsg("$Assassin07$", pTeacher);
}

func Script27()
{
  ShoutMsg("$Assassin08$", pTeacher);  
}

func Script28()
{
  ShoutMsg("$Assassin09$", pTeacher);
}

func Script30()
{
  ShoutMsg("$Assassin10$", pClonk);
}

func Script31()
{
  ShoutMsg(["$Assassin11$","$Assassin12$", "$Assassin13$"], pTeacher);
}

func Script32()
{
  ShoutMsg(["$Assassin14$","$Assassin15$","$Assassin16$"], pTeacher);
}

func Script33()
{
  ShoutMsg(["$Assassin17$", "$Assassin18$"], pTeacher);
  fCrossbow = 1;
  var iPlr;
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    iPlr = GetPlayerByIndex(i,C4PT_User);
    CreateContents(NBTP, GetCursor(iPlr));
    CreateContents(BOTP, GetCursor(iPlr));
    CreateContents(HOOK, GetCursor(iPlr));
    CreateContents(CRBW, GetCursor(iPlr));
  }
}

func Script50()
{
  if(!FindObject2(Find_ID(ASAS), Find_InRect(411, 0, LandscapeWidth()-441, 440))) return goto(49);
  ShoutMsg("$Assassin19$", pClonk);
}

func Script51()
{
  ShoutMsg("$Assassin20$", pTeacher);
}

func Script52()
{
  ShoutMsg(["$Assassin21$","$Assassin22$","$Assassin23$"], pTeacher);
}

func Script53()
{
  ShoutMsg(["$Assassin24$","$Assassin25$"], pTeacher);
}

func Script54()
{
  ShoutMsg(["$Assassin26$","$Assassin27$"], pTeacher);
  fKnifes = 1;
  var iPlr;
  for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
  {
    iPlr = GetPlayerByIndex(i,C4PT_User);
    CreateContents(KNFP, GetCursor(iPlr));
  }
}

func Script55()
{
  ShoutMsg(["$Assassin28$","$Assassin29$"], pTeacher);
}

func Script56()
{
  ShoutMsg("$Assassin30$", pClonk);
  ScriptGo(0);
}



protected func RelaunchPlayer(iPlr)
{
  if(GetPlayerType(iPlr)!=C4PT_User) return;
  var id = ASAS;
  var crew = CreateObject(id,67, 789,iPlr);
  MakeCrewMember(crew,iPlr);
  SetCursor(iPlr, crew);
  DoEnergy(100, crew);
  if(fCrossbow)
  {
    CreateContents(HOOK, crew);
    CreateContents(BOTP, crew);
    CreateContents(NBTP, crew);
    CreateContents(CRBW, crew);
  }
  if(fKnifes) CreateContents(KNFP, crew);
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

public func MsgSignpost1()  { return "$Assassin31$"; }
