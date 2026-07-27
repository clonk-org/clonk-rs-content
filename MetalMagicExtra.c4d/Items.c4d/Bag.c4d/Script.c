/*-- Alchemiebeutel --*/

#strict 2

local idShow;

protected func Activate(pCaller)
{
  [$CtrlActivateBag$]
  var pBag = pCaller;
  // Clonk hat schon eine Tasche?
  if(AlchemBag(pBag)) 
  {
    // ist da was drin? Dann ausleeren (aber nicht unter Wasser)
    if(AlchemCount() && pBag->~Ready2Decompose()) {
      pBag->Transfer(this);
      return 1;
    }
    // ansonsten Meldung dass er schon einen hat
    PlayerMessage(GetOwner(pCaller), "$MsgAlreadyHasBag$", pCaller, pCaller->GetName());
    return 1;  
  }   
  // an den Clonk heften
  if (!BelongTo(pCaller)) return true;
  Sound("Connect",0,0,0,GetOwner(pCaller)+1);
  PlayerMessage(GetOwner(pCaller),"$MsgPuton$",pCaller);
  return 1;
}

public func Exhaustion()
{
  // angeheftet muss der Beutel dableiben
  if(GetAction()=="Belongs") return true;
  // im Inventar, etc
  return true;
}

public func BelongTo(object pObject) { SetAction("Belongs",pObject); SetCategory(GetCategory()|C4D_MouseIgnore); return true; }
public func AbortBelong() { SetCategory(GetCategory(0, GetID())); }

// Menü für Alchemie, das im Clonk auftaucht, an den der Beutel gehaengt ist
protected func AttachContextAlchem(object pActionTarget, object pObj, int iSelection)
{
  [$CtxAlchemy$|Image=ALC_]

  // Clonk soll anhalten
  pActionTarget->SetComDir(COMD_Stop);

  var i, j, idAlchem;
  // Kontextmenü erzeugen
  CreateMenu(ALC_,pActionTarget,0,0,"$CtxAlchemy$",0,1,0);

  // zerlegen/entleeren (nur wenn nicht Contained())
  if(!Contained(pActionTarget) && Ready2Decompose()) {
    var pAlchemContainer, idAlchem, szName;
    for(;pAlchemContainer=AlchemContainer(i,pActionTarget);++i) {
      if(pAlchemContainer->~AlchemProcessTime() == 0) { // was zum rausnehmen
        if(AlchemCount(pAlchemContainer)) // Inhalt?
          AddMenuItem(Format("$MnuExhaust$",GetName(pAlchemContainer)),"Decompose",GetID(pAlchemContainer),pActionTarget,0,pAlchemContainer);
      } else {
        AddMenuItem(Format("$MnuDecompose$",GetName(pAlchemContainer)),"Decompose",GetID(pAlchemContainer),pActionTarget,0,pAlchemContainer);
      }
    }
  }

  AddMenuItem("$MnuIndex$","Index",ALC_,pActionTarget);
  if(idShow) AddMenuItem("$MsgRemoveCostShower$","RemoveRitual",BMSD,pActionTarget,0,1,0,2,1);
  else if(FindObject2(Find_ID(CLST)))  AddMenuItem("$MsgCostShower$","Ritual",BMSD,pActionTarget,0,0,0,2,1);
  AddMenuItem("$MnuDeposit$","Loose",BMSD,pActionTarget);
  AddMenuItem("$MnuHelp$","HelpTxt",CXIN,pActionTarget);

  return 1;
}

protected func RemoveRitual(idObj, fShowMenu)
{
  // Nachrichteneffekte entfernen
  while(GetEffect("CheckRitualMessage",GetActionTarget()))RemoveEffect("CheckRitualMessage",GetActionTarget());
  idShow = 0;
  CustomMessage("", 0, GetOwner(GetActionTarget()));
  if(fShowMenu)
    AttachContextAlchem(GetActionTarget());
}

protected func Ritual()
{
  var pClonk = GetActionTarget();
  var idRitual, i,iChurch = MMGetChurch(GetOwner(pClonk));

  CreateMenu(CLST,pClonk,0,0,"$MsgNoRituals$",0,0,0);
  // Alle Baupläne durchsuchen
  while(idRitual = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_All))
  {
    // Rituale rausfiltern
    if(idRitual->~IsRitual())
    {
      // Nur bei passender Kirche aufnehmen
      if(idRitual->~NeedChurch())
      {
        if(iChurch == idRitual->~NeedChurch()) AddMenuItem("$MsgRitual$: %s","ShowRitual",idRitual,pClonk,);
      }
      else AddMenuItem("$MsgRitual$: %s","ShowRitual",idRitual,pClonk,);
    }
  }
}

protected func ShowRitual(idID,bUpdate)
{
  idShow = idID;  
  // Verfügbarkeit überprüfen
  var pTemp;
  var pClonk = GetActionTarget();
  if(!ObjectCount(ALCO)) pTemp = CreateObject(ALCO,10,10,-1);
  var fCheck = CheckAlchem(idID, pClonk);
  if(pTemp) RemoveObject(pTemp);
  
  // Bisherige Nachrichteneffekte entfernen
  if(!bUpdate)
   while(GetEffect("CheckRitualMessage",pClonk))RemoveEffect("CheckRitualMessage",pClonk);
  
  if(!fCheck) {
    var adString;
    adString=Format("@$MsgRitualComponentsNeeded$",GetSpecNeededAlcStr(idID,pClonk), GetName(0,idID));
    PlayerMessage(GetOwner(pClonk), adString);
    
    if(!bUpdate)
     AddEffect("CheckRitualMessage",pClonk,1,10,this,0,adString,this);
    return 0;
  }
  PlayerMessage(GetOwner(pClonk),Format("$MsgRitualEnoughtComponents$", GetName(0,idID)));
  
  if(!bUpdate)
   idShow = 0;
}

public func GetSpecNeededAlcStr(id idMagic) { // Par(1)-Par(x): Zaubernde, [Zauberturm], [weitere...]

    // Par(1) = this wenn =0
    if(!Par(1)) Par(1)=this;

    var idAlchem,iDesire, i, m, szString, szSplit;
    szString = "";

    // Components in der Defcore einmal durchgehen...
    for(i=0; GetComponent(0,i,0,idMagic); ++i) {

      // Von jedem Eintrag ID und Anzahl merken
      idAlchem = GetComponent(0,i,0,idMagic);
      iDesire = GetComponent(idAlchem,0,0,idMagic);

      // Alle Objekte der Parameterliste nach der gesuchten Zutat durchsuchen...
      for(m=1;Par(m);++m) {
        iDesire -= GetAlchem(idAlchem,0,Par(m));
        if(iDesire <= 0) break;
      }
          // Alle Objekte durchsucht aber nicht genug da? String anhängen
      if(iDesire > 0) {
        // erstes Mal keinen Seperator
        if(szString != "") szSplit = ", ";
        else szSplit = "";
        szString = Format("%s%s%dx{{%i}}",szString,szSplit,iDesire,idAlchem);
      }

    }
    return szString;
}

func FxCheckRitualMessageStart(pTarget,iEffectNumber,iTemp,par1,par2)
{
 if(iTemp)return;
 EffectVar(0,pTarget,iEffectNumber)=par1;
 EffectVar(1,pTarget,iEffectNumber)=par2;
}

func FxCheckRitualMessageTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(GetCursor(GetOwner(pTarget)) != pTarget)
 {
  return -1;
 }
 return 1;
}

func FxCheckRitualMessageStop(pTarget,iEffectNumber,iReason,iTemp)
{
 if(iTemp)return;
 PlayerMessage(GetOwner(pTarget), "");
 LocalN("idShow",EffectVar(1,pTarget,iEffectNumber))=0;
 return;
}

protected func SomethingAlchem()
  { return AlchemCount() || AlchemContainerCount(GetActionTarget()); }

private func Decompose(id idType, object pObject) {
  // sofort zerlegen
  if(pObject->~AlchemProcessTime() == 0) Transfer(pObject);
  else AddEffect("IntDecompose",GetActionTarget(),1,1,0,GetID(),pObject,this);
}

public func Loose() {
  PlayerMessage(GetOwner(GetActionTarget()),"$MsgDeposit$",GetActionTarget());
  var pEnter = Contained(GetActionTarget());
  SetAction("Idle");
  SetActionTargets();
  if(pEnter) Enter(pEnter);
}

public func HelpTxt() {
  MessageWindow("$MsgHelpTxt$",GetOwner(GetActionTarget()),CXIN,"$MnuHelp$");
}

public func Index() {
  // keine Zutaten vorhanden
  if(!AlchemCount()) {
    MessageWindow("$MsgEmpty$",GetOwner(GetActionTarget()),ALC_,"$MnuIndex$");
  }
  // Zutaten vorhanden
  else {
    CreateMenu(ALC_,GetActionTarget(),0,0,"$MnuIndex$",0,1,1);
    var idAlchem;
    for(var i=0; idAlchem=GetAlchem(0,i); ++i)
      AddMenuItem(GetName(0,idAlchem),"And()",idAlchem,GetActionTarget(),GetAlchem(idAlchem));
  }
}

func FxIntDecomposeStart(object pClonk, int iNumber, int iTemp, object pAlchemContainer, object pAlchemManager)
{
  var szAct;
  EffectVar(0,pClonk,iNumber) = pAlchemContainer;
  EffectVar(1,pClonk,iNumber) = pAlchemManager;
  // Process vorhanden? toll. Nicht vorhanden? nich so toll.
  // ...aber möglich!! :)
  if(GetActMapVal("Name","Process",GetID(pClonk)))
    szAct = "Process";
  else
    szAct = "FlatUp";
  pClonk->SetAction(szAct);
  EffectVar(2,pClonk,iNumber) = szAct;
  // Objekt "virtuell" vor sich hinlegen
  var iOvID = EffectVar(3,pClonk,iNumber) = GetUnusedOverlayID(GFXOV_ProcessTarget, pClonk);
  SetGraphics(0, pClonk, 0, iOvID, GFXOV_MODE_Object, 0, GFX_BLIT_Parent, pAlchemContainer);
  SetObjDrawTransform(1000, 0, GetDir(pClonk)*20000-10000, 0, 1000, 12000 - 1000 * (GetDefBottom(pAlchemContainer) - GetY(pAlchemContainer)), pClonk, iOvID);
}

func FxIntDecomposeTimer(object pClonk, int iNumber, int iEffectTime) {
  var pAlchemContainer = EffectVar(0,pClonk,iNumber),
      pAlchemManager = EffectVar(1,pClonk,iNumber),
      szAction = EffectVar(2,pClonk,iNumber);

  // Objekt weg
  if( !pAlchemContainer || Contained(pAlchemContainer) != pClonk)
  {
    pClonk->SetAction("KneelUp");
    return -1;
  }

  // Beutel weg
  if(!pAlchemManager || !GetAlchemBag(pClonk))
  {
    pClonk->SetAction("KneelUp");
    return -1;
  }

  // Zeit abgelaufen
  if(iEffectTime >= pAlchemContainer->~AlchemProcessTime())
  {
    pAlchemManager->Transfer(pAlchemContainer);
    return -1;
  }

  // Aktion
  if(szAction == "FlatUp")
    if(GetPhase(pClonk)!=4) SetPhase(4,pClonk);
  if(GetAction(pClonk) != szAction) return -1;
}

func FxIntDecomposeStop(object pClonk, int iNumber, int iReason, bool fTemp)
{
  if (fTemp) return;
  // Process fertig: Overlay immer entfernen
  // (bei Erfolg wird es ohnehin schon entfernt sein, da das Objekt gelöscht wurde)
  SetGraphics(0, pClonk, 0, EffectVar(3,pClonk,iNumber));
  return FX_OK;
}

public func Transfer(object pObject) {

  // Clonk wieder hinstellen
  if(GetAction(GetActionTarget()) != "Walk")
    GetActionTarget()->SetAction("KneelUp");

  // bei leerem Behälter bestimmte Nachricht ausgeben
  if(!GetAlchem(0,0,pObject))
    PlayerMessage(GetOwner(GetActionTarget()),"$MsgContainerEmpty$",this);
  else
    PlayerMessage(GetOwner(GetActionTarget()),Format("$MsgReceived$.",TransferAlcStr(pObject)),this);
  // und nun den Inhalt rüberschieben
  TransferAlchem(pObject,this);
  // Ritual updaten, wenn nötig
  if(idShow) ShowRitual(idShow,true);
 }

private func TransferAlcStr(object pObject, int iMul) {
  var idAlchem, iAlchem, szMessage;

  if(!iMul) iMul = 1;
  szMessage = "";

  // alle Zutaten
  for(var i=0; idAlchem=GetAlchem(0,i,pObject); ++i)
  {
    iAlchem = GetAlchem(idAlchem,0,pObject)*iMul;

    // tatsächlich gar keine Zutat sondern ein weiterer Behälter?
    if(idAlchem->~IsAlchemContainer()) {
      var pObj = CreateObject(idAlchem,0,0,-1);
      szMessage = Format("%s%s",szMessage,TransferAlcStr(pObj,iAlchem));
      if(pObj) RemoveObject(pObj);
    }

    else {
      szMessage = Format("%s|%dx{{%i}}%s",szMessage,iAlchem,idAlchem,GetName(0,idAlchem));
    }
  }
  return szMessage;
}

protected func Ready2Decompose() { if(GetActionTarget()) if(GetAction(GetActionTarget()) == "Walk") return 1;  }

protected func RejectEntrance() { if(GetAction()=="Belongs") return 1; }

public func IsAlchemContainer() { return true; }

protected func Hit() { Sound("WoodHit*"); }
