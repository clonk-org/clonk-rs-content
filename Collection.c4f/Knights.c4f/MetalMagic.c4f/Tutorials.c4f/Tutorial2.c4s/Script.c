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
  ShoutMsg("Ist jemand zu Hause? Ich bin ein Magier und wollte...", pClonk);
}

func Script22()
{
  pMage = CreateObject(MAGE, 228, 577, -1);
  SetColorDw(RGB(200,200,0), pMage);
  SetName("Myxus", pMage);
  SetDir(0, pMage);
  PlacementEffects(pMage->GetX(), pMage->GetY());
  ShoutMsg("Was wollt ihr in meinem Reich, einfacher Magier?", pMage);
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
  ShoutMsg("Ich will alles über die Kampfmagier des Rates lernen.", pClonk);
}

func Script24()
{
  ShoutMsg("Ah ihr seid wissbegierig. Dann seid ihr bei mir genau richtig. Ich weiß alles über den Rat von Cra Kla Zoth.", pMage);
}

func Script25()
{
  ShoutMsg("Also. Der Rat wurde 118 als Zusammenschluss der weisesten Magier des Landes gegründet um dem schon seit Jahren tobenden Glaubenskieg zwischen dem Orden der Sawelô und der Legion des Teiwaz Einhalt zu bieten.", pMage);
}

func Script26()
{
  ShoutMsg(["Die drei großen Gründer hießen Cra Kla und Zoth. Deswegen bekommen die Vorsitzenden des Rates seither diese Namen als Titel.", "Nur sehr ungebildete Laien glauben die drei würden schon seit über Hundert Jahren den Rat leiten."], pMage);
}

func Script27()
{
  ShoutMsg("Die Macht des Rates gründet sich auf den Stab von Cra Kla Zoth. Ein magischer Stab, in dem alle 4 Elemente vereinigt sind.", pMage);  
}

func Script28()
{
  ShoutMsg(["Soweit zur Theorie. Jetzt kannst du dich als Magus ausbilden lassen. Gehe dazu in das Magiezimmer und bilde dich dort aus.", "Du findest es im Obergeschoss bei der Magusstatue. Stelle dich zwischen Schreibpult und Fackel und drücke nach oben. Dann wird dir die Statue den Zugang öffnen."], pMage);
}

func Script30()
{
  if(!FindObject2(Find_ID(MAGE), Find_PlayerType(C4PT_User))) return goto(29);
  ShoutMsg(["So, das hast du ja schon geschafft. Nun brauchst du einen Stab.","Er ermöglicht es spezielle Stabzauber zu wirken.", "Außerdem spendet er noch Mana solange die Regel Zauberenergienachschub {{STES}} aktiviert ist."], pMage);
  CreateObject(STES, 10, 10, -1);
}

func Script31()
{
  ShoutMsg(["Die Stäbe haben jeweils immer zwei Angriffszauber {{_CSB:3}} {{_CSB:8}}, einen Nahkampfzauber {{_CSB:5}}, eine Aura {{_CSB:4}}, einen Verteidigungszauber {{_CSB:7}} sowei einen Spezialzauber {{_CSB:6}}.", "Die Tastenangaben sind die Tasten für das Kombomenü. Beim festen Kombomenü ist Vorne Rechts und Zurück Links.", "Im Schnellzaubermenü haben die Zauber auch eine festgelegte Reihenfolge: Aura, Verteidiung, Angriff, Spezial, Nahkampf und darunter in der Mitte der zweite Angriffszauber."], pMage);
  CreateObject(STES, 10, 10, -1);
}

func Script32()
{
  ShoutMsg(["Diese sind im normalen Zaubermenü ganz oben, bzw. im Schnellzaubermenü unter den normalen Schnellzaubern.", "Im Kombomenü bilden die Stabzauber einen ersten Komboring. Mit einem weiteren Klick auf {{_CSB:11}} öffnet sich das normale Kombomenü mit den Nicht-Stabzaubern."], pMage);
}

func Script33()
{
  ShoutMsg(["Wenn die Regel 'Stäbe für Magi' {{FSTF}} aktiviert ist, dann bekommst du automatisch als Magus einen Stab.","Ansonsten kannst du dir den Stab im Magiezimmer bauen und dann auf Doppelgraben eine Elementarvariante auswählen."], pMage);
}

func Script34()
{
  ShoutMsg(["Heute bekommst du die Stäbe aber in dem Übungsparcours, den ich schon vorbereitet habe.","Dazu musst du einfach zu den Zaubersteinen gehen.", "Der erste ist der weiße Stein unter der Burg.", "P.S. die Zauber müssen immer in der gleichen Reihenfolge eingesetzt werden, in der ich sie dir erkläre."], pMage);
}

func Script35()
{
  ShoutMsg("Aha gut. Danke für deine Hilfe. Ich werde nun versuchen den Parcours zu meistern.", pClonk);
}

func Script36()
{
  ScriptGo(0);
}

func StaffSMARActivated()
{
  ShoutMsg(["Ihr habt nun den Luftstab. Dieser ermöglicht es folgende Zauber zu wirken:","{{CHCL}} Farbwechsel: Damit kann man sich mit der Spielerfarbe, Name oder Typ eines anderen Clonks tarnen, um so den Gegner zu täuschen.", "{{MDFL}} Zurückwerfen: Damit kann man sich gut vor Wurfobjekten schützen, die dann auf den Gegner zurückfliegen."], pMage);
  goto(100);
}

func Script101()
{
  ShoutMsg(["{{AIPA}} Pfeilabwehr: Diese Aura schützt alle im Umkreis vor Pfeiltreffern.", "{{MLGT}} Magischer Blitz: Ein starker Blitz wird beschworen. Dieser ist besonders effektiv gegen Eiskrähen."], pMage);
}

func Script102()
{
  ShoutMsg(["{{PRFT}} Druckfaust: Im Nahkampf werden Gegner zurückgeschleudert. Allerdings macht der Clonk dann keinen Schaden mehr im Nahkampf.", "{{AFST}} Luftklaue: Ein starker Angriffszauber, der mehrere Gegner schädigen kann und durch Wände gehen kann."], pMage);
}

func Script103()
{
  ScriptGo(0);
}

func StaffSMERActivated()
{
  ShoutMsg(["Als nächstes kommt der Erdstab dran:","{{MSSH}} Steinschild: Schützt den Clonk vor Schaden.", "{{FGOB}} Objektschleuder: Wirft das erste Inventarobjekt des Clonks. Je Leichter dieses ist, desto weiter fliegt es."], pMage);
  goto(200);
}

func Script201()
{
  ShoutMsg(["{{AUEQ}} Erdbebenaura: Als Objekte im Umkreis werden durchgeschüttelt und fliegen durch die Luft. Erneutes Zaubern deaktiviert die Aura.", "{{SHFT}} Schattenfaust: Der Clonk schlägt im Nahkampf viel schneller zu."], pMage);
}

func Script202()
{
  ShoutMsg(["{{MARK}} Steinschlag: Schleudert einige Steine. Diese machen zwar wenig Schaden, können Gegner aber gut wegschleudern.", "{{MPXC}} Platzwechsel: Der Clonk kann mit einem verbündeten Clonk den Platz wechseln. Ist dieser nicht in direkter Sichtweite ist die Reichweite stark eingeschränkt. Trotzdem muss der Clonk jedoch erreichbar sein."], pMage);
}

func Script203()
{
  ScriptGo(0);
}

func StaffSMFRActivated()
{
  ShoutMsg(["Jetzt ist es Zeit für den Feuerstab:","{{MHTB}} Heißes Blut: Der Clonk beginnt zu brennen ohne Schaden zu nehmen. Somit kann er gut Gegner anzünden.", "{{MINC}} Anzünden: Zündet das erste Inventarobjekt des Clonkes an, wenn es brennbar ist. Damit kann man z.B. Flammenspeere werfen."], pMage);
  FreeRect(1340,640,12,40);
  goto(300);
}

func Script301()
{
  ShoutMsg(["{{AUHE}} Hitzeaura: Alle Clonks im Umkreis erleiden Schaden. Aber auch eine Clonks sowie der Zauberer bekommen Schaden. Durch erneutes Zaubern lässt sie sich wieder abstellen.", "{{FLFT}} Feuerfaust: Der Clonk schlägt im Nahkampf mit kräftigen Flammenhieben zu."], pMage);
}

func Script302()
{
  ShoutMsg(["{{MFCS}} Feuerkluster: Schleudert drei Feuerprojektile. Diese sind sehr schwach, lassen sich dafür aber gut in Massen zaubern.", "{{MFBL}} Feuerklumpen: Ein brennedes Stück Feuer, das man auf Gegner werfen kann."], pMage);
}

func Script303()
{
  ScriptGo(0);
}

func StaffSMWTActivated()
{
  ShoutMsg(["Als letzes kommt der Wasserstab:","{{RCTL}} Rettungsteleport: Der Clonk wird sofort zur nächsten Basis gebeamt. Dabei verliert er allerdings sein Inventar. PS oben auf einer Himmelsinsel ist eine Basis.", "{{MFBZ}} Frosthauch: Schleudert Clonks weg und friert sie ein."], pMage);
  goto(400);
}

func Script401()
{
  ShoutMsg(["{{MGSW}} Schneeball: Frostiges Projektiel, das dem Gegner schadet und einfriert.", "{{MCIB}} Flaschenclonk: Der Clonk wird in eine Flasche gesteckt. Praktisch zum Transport. Erneutes Zaubern befreit den Clonk wieder."], pMage);
}

func Script402()
{
  ShoutMsg(["{{MHGA}} Heilungsaura: Heilt alle verbündeten Clonks im Umkreis. Auch der Zauberer wird dabei geheilt.", "{{ICFT}} Eisfaust: Der Nahkampfgegner wird wärend dem Kampf eingefroren."], pMage);
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
  AddMenuItem("Weiter", "ScriptGo(1)", LMM2, obj, 0, obj, 0, 2, 4);
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
        Message("Ah gut. Ein Aquaclonk.|Dir mache ich natürlich gerne das Tor auf.", pAqua);
        Local(0, pAqua)->SetComDir(COMD_Up);
	FindObject(RCTP)->Activate();
        return -1;
      }
    Message("Ich mache nur Aquaclonks das Tor auf!|Alle andern Clonks sind doof.", pAqua);
  }
}

global func FxSpecDyingClonkTimer(pClonk, iNumber)
{
  var pClonk, iEff, pTest;
  if(pTest = FindObject2(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Exclude(pClonk), Find_Distance(250), Sort_Distance()))
  {
    if(GetEnergy(pClonk)>10)
      {
        Message("Ah, es geht mir wieder besser. Ich mach dir das Tor auf.", pClonk);
        ObjectSetAction(pClonk, "Walk", 0, 0, 1);
        SetCommand(pClonk, "MoveTo", 0, GetX(pClonk)-35, GetY(pClonk));
        return -1;
      }
    Message("Args ich sterbe! Hilft mir denn keiner?", pClonk);
  }
}

public func MsgSignpost1()  { return "Weg zum Assassinenlager|Du bist am Ziel."; }

public func SignpostReached()
  {
  SetNextMission("MetalMagic.c4f\\Tutorials.c4f\\Tutorial3.c4s", "$BtnNextRound$", "$BtnNextRoundDesc$");
  }
  
