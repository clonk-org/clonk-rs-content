/*-- Kanderianer --*/

#strict 2

#include KNIG

local fGrabbedInfo;

// Der Kanderier benutzt immer die InfoSektion eines Ritters
protected func Recruitment()
{
  if(!fGrabbedInfo)
    {
    // Ritter erstellen
    var cb = CreateObject(KNIG, 0, 10, GetOwner());
    MakeCrewMember(cb, GetOwner());
    // InfoSektion klauen
    fGrabbedInfo = true;
    GrabObjectInfo(cb);
    // Den anderen Clonk wieder löschen
    RemoveObject(cb);
    // Volles Leben
    DoEnergy(100);
    // Neues Portrait
    AdjustPortrait();
    }
  // Weiter im Text
  return _inherited();
}

/* Portraits */

// Gibt Anzahl an Standard-Portraits an
public func StdPortraitCount() { return 3; }

private func AdjustPortrait()
{
  // Mein Portrait stammt auch nicht von mir?
  if(GetPortrait(this, true) != GetID())
  {
    // Neues Portrait setzen
    var iRandoms = Random(StdPortraitCount()) + 1;
    SetPortrait(Format("%d", iRandoms), this, GetID());
  }
}

public func StartSpearAttack(pSpear)
{
 if(!WildcardMatch(GetAction(),"*SpearWalk*")) return;
 
 if(GetAction() == "SpearWalk") SetAction("ProtectedSpearAttack");
 
 return 1;
}

public func CanUseSpearAttack() { return 1; }

private func SpearAttack()
{
 if(GetPhase() != 6)return;
 var dir=-1;
 if(GetDir() == DIR_Right)dir=1;
 
 for(var clnk in FindObjects(Find_AtPoint(10*dir,0),Find_OCF(OCF_Alive)))
 {
  Punch(clnk,10+Random(5));
 }
}

public func ContextKneelShield(pCaller)
{
  [$CtxKneelShieldDesc$|Image=SHI2|Condition=HasShield]
  if(!WildcardMatch(GetAction(),"*Walk*"))return;
  if(WildcardMatch(GetAction(),"*Spear*"))
   SetAction("KneelShieldSpear");
  else
   SetAction("KneelShield");
  
  SetPhase(2);
}

protected func ControlDown(object pByObject)
{ 
  SetOrderNone(); 
  if(IsFighting() ) 
  { 
    StopFighting(); 
    return 1; 
  } 
  if(!pByObject) pByObject = this;
  if ( Control2Vehicle("ControlDown", pByObject) ) return 1; 
  if ( Control2Contents("ControlDown", pByObject) ) return 1; 
  
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   SetPhase(BoundBy(GetPhase()-1,0,GetActMapVal("Length",GetAction(),GetID())-1));
   return 1;
  }
  
  return _inherited(pByObject); 
} 

protected func ControlUp(object pByObject)
{ 
  SetOrderNone(); 
  if(!pByObject) pByObject = this;
  if ( Control2Vehicle("ControlUp", pByObject) ) return 1; 
  if ( Control2Contents("ControlUp", pByObject) ) return 1; 
  
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   SetPhase(BoundBy(GetPhase()+1,0,GetActMapVal("Length",GetAction(),GetID())-1));
   return 1;
  }
  
  return _inherited(pByObject); 
} 

protected func ControlLeftDouble(object pByObject) 
{ 
  SetOrderNone(); 
  if(!pByObject) pByObject = this;
  if ( Control2Vehicle("ControlLeftDouble", pByObject) ) return 1; 
  if ( Control2Contents("ControlLeftDouble", pByObject) ) return 1; 
  
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   SetAction("Walk");
   SetComDir(COMD_Left);
   return 1;
  }
  
  return _inherited(pByObject); 
} 

protected func ControlRightDouble(object pByObject)
{ 
  SetOrderNone(); 
  if(!pByObject) pByObject = this;
  if ( Control2Vehicle("ControlRightDouble", pByObject) ) return 1; 
  if ( Control2Contents("ControlRightDouble", pByObject) ) return 1; 
  
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   SetAction("Walk");
   SetComDir(COMD_Right);
   return 1;
  }
  
  return _inherited(pByObject); 
} 

protected func ControlLeft(object pByObject) 
{ 
  SetOrderNone(); 
  if(!pByObject) pByObject = this;
  if ( Control2Vehicle("ControlLeft", pByObject) ) return 1; 
  if ( Control2Contents("ControlLeft", pByObject) ) return 1; 
  
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   SetDir(DIR_Left);
   return 1;
  }
  
  return _inherited(pByObject); 
} 
protected func ControlRight(object pByObject) 
{ 
  SetOrderNone(); 
  if(!pByObject) pByObject = this;
  if ( Control2Vehicle("ControlRight", pByObject) ) return 1; 
  if ( Control2Contents("ControlRight", pByObject) ) return 1; 
  
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   SetDir(DIR_Right);
   return 1;
  }
  
  return _inherited(pByObject); 
} 

private func ChangeBowAimAngle(iChange) 
  { 
  var iOldAngle; 
  // Derzeitigen Winkel ermitteln 
  if (IsAiming()) iOldAngle = GetPhase(); 
  else
  if (IsLoading()) iOldAngle = iLastAimAngle; 
  else return false;
  // Am Anschlag? 
  if (!Inside(iOldAngle + iChange, 0, 7)) return; 
  // Neuen Winkel setzen 
  if (IsAiming()) {SetPhase(iOldAngle + iChange); return true;}
  if (IsLoading()) {iLastAimAngle += iChange; return true;}
  } 

public func BowControlUp(pBow, &rVal) 
  { 
  rVal=ChangeBowAimAngle(-1); 
  // Funktion überladen 
  return 1; 
  } 
 
public func BowControlDown(pBow, &rVal) 
  { 
  rVal=ChangeBowAimAngle(+1); 
  // Funktion überladen 
  return 1; 
  } 

/*--------------------------------------------- Status -------------------------------------------------*/

//! Welche Objekte kann dieser Clonk einsetzen?
public func CanUse(idObj)
{
  // Schwert
  if (idObj == SCMT) return 1;
  // Axt
  if (idObj == AXE1) return 1;  
  // Bogen 
  if (idObj == BOW1) return 1;   
  // Schild 
  if (idObj == SHI2) return 1;   
  // Speer 
  if (idObj == SPER) return 1;   
  // Lanze 
  if (idObj == LANC) return 1;    
  // Unbekannt 
  return 0; 
} 
 
//! Bevorzugte Waffentypen für selbständige Bewaffnung (zur Zeit werden genau zwei Waffentypen erwartet). 
public func GetFightWeaponType(index) 
{ 
  if (index == 0) return SPER;
  if (index == 1) return SWOR; 
  if (index == 2) return AXE1; 
  return 0; 
} 
 
//! Dieser Clonk ist kein Ritter
public func IsKnight()  
{  
  return 0;  
} 
 
//! aber ein Kanderier
public func IsKanderian()
{
  return 1;
}

//! Der Clonk gilt als bewaffnet, wenn er mindestens ein Schild nebst einer Handwaffe trägt.
private func IsFullyArmed() 
{ 
  return (HasFightWeapon() && HasShield()); 
} 
 
//! Nicht voll bewaffnet. 
public func IsNotFullyArmed()  
{  
  return (!IsFullyArmed());  
} 
 
//! Trägt der Clonk einen Bogen und hat Pfeile dabei? 
private func IsBowEquipped() 
{ 
  // Bogen dabei? 
  if (!FindContents(BOW1)) return 0; 
  // Pfeile dabei? 
  if (!GetArrowCount()) return 0; 
  // Bogen und Pfeile dabei 
  return 1; 
} 
 
//! Liefert den Schutzfaktor. 
public func IsShielded(fArrow) 
{ 
  var iShielded = 0, iShieldFactor; 
  
  // Kleiner Standardschutz gegen Pfeile
  if(fArrow)
   iShielded+=5;
  
  // Schild 
  var pShield; 
  if (pShield = HasShield()) 
    // Benutzerdefiniertes Schild 
    if (iShieldFactor = pShield->~ShieldFactor(fArrow)) 
      iShielded += iShieldFactor; 
    // Normales Schild 
    else 
      // Das Standardschild hat einen Bonus gegen Pfeile 
      if (fArrow) 
        iShielded += 20; 
      else 
        iShielded += 10; 
  // Rüstung                    
  if (HasArmor()) 
    iShielded += DefinitionCall(idArmorType, "ShieldFactor"); 
  
  // Bei entsprechender Aktion
  if(WildcardMatch(GetAction(),"*Protected*"))
   iShielded+=40;
  // Wert zurückliefern 
  return iShielded; 
} 
 
//! Prüft ob pObj (oder dieser Clonk) ein Schild trägt. 
public func HasShield(pObj)  
{  
  if (!pObj) pObj = this;  
  var obj;
  if(obj=FindObject(0,0,0,0,0,0, "Shield", pObj))
   return (obj);  
  return idShield;
} 
 
//! Prüft ob pObj (oder dieser Clonk) eine Lanze trägt. 
public func HasLance(pObj)  
{  
  if (!pObj) pObj = this;  
  return (FindObject(LNCA, 0,0,0,0,0, "Lance", pObj));  
} 
 
//! Prüft ob der Clonk eine Handwaffe trägt, die er verwenden kann. 
public func HasFightWeapon(pClonk) 
{  
  // Anderer Clonk 
  if (pClonk)  
    return (pClonk->~HasFightWeapon(this)); 
  // Dieser Clonk 
  var pObj; 
  for (var i = 0; pObj = Contents(i); i++) 
    if (pObj->~IsFightWeapon(this) && CanUse(GetID(pObj))) 
      return 1;
  // Keine Waffe 
  return 0; 
} 
 
//! Liefert die erste gefundene Handwaffe, die der Clonk verwenden kann. 
public func GetFightWeapon(pClonk) 
{  
  // Anderer Clonk 
  if (pClonk)  
    return (pClonk->~GetFightWeapon()); 
  // Dieser Clonk 
  var pObj; 
  for (var i = 0; pObj = Contents(i); i++) 
    if (pObj->~IsFightWeapon(this) && CanUse(GetID(pObj))) 
      return pObj; 
  // Keine Waffe 
  return 0; 
} 

  
private func ExecuteOrderArm() 
  { 
  // Schild dabei: anlegen 
  var pWeapon; 
  if (!HasShield()) 
    if (pWeapon=FindContents(SHI2)) 
      return ProtectedCall(pWeapon,"Activate",this); 
    else 
      // Noch kein Schild: suchen 
      if (pWeapon=FindAvailableWeapon(SHI2)) 
        return SetCommand(this,"Get",pWeapon); 
  // Noch keine Handwaffe: suchen 
  if (!HasFightWeapon()) 
    { 
    var idWp1,idWp2,idWp3; 
    
    for(var cnt=1;cnt<4;cnt++)
     VarN(Format("idWp%d",cnt))=GetFightWeaponType(cnt-1);
    
    if(Random(2))
     pWeapon = FindAvailableWeapon(idWp1); 
    else pWeapon = FindAvailableWeapon(idWp2); 
    if (!pWeapon) pWeapon = FindAvailableWeapon(idWp3); 
    if (pWeapon) return SetCommand(this,"Get",pWeapon); 
    } 
  // Wenn's geht noch einen Bogen suchen 
  if (!FindContents(BOW1)) 
    { 
    if (pWeapon=FindAvailableWeapon(BOW1)) 
      return SetCommand(this,"Get",pWeapon); 
    } 
  else 
    // Hat einen Bogen: noch Pfeile dazu suchen 
    if (GetArrowCount()<21) 
      if (pWeapon=FindAvailableArrow())
        return SetCommand(this,"Get",pWeapon);   
  // Wenn's geht noch einen Speer suchen 
  if (!FindContents(SPER)) 
    if (pWeapon=FindAvailableWeapon(SPER)) 
      return SetCommand(this,"Get",pWeapon); 
  // Fehlgeschlagen 
  if (!IsFullyArmed()) return OrderFailure("$MsgNoWeapons$"); // "Keine Waffen." 
  // Fertig   
  return OrderSuccess("$MsgArmed$"); // "Bewaffnet." 
  }

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Der Ritter kann zusätzlich noch 30 Pfeile tragen
  if(iIndex == 0) { if(fAmount) return(30); return("IsArrow"); }
}
 
protected func GetObject2Drop(pCollectObj) 
  { 
  var iArrowPriority; 
  // Wenn ein Pfeil oder Pfeilpaket aufgenommen werden soll... 
  if (pCollectObj) if (ObjectCall(pCollectObj, "IsArrow") || ObjectCall(pCollectObj, "IsArrowPack")) 
    // Pfeilen und Pfeilpaketen eine niedrigere Priorität geben 
    iArrowPriority=-500; 
  // Das schlechteste Objekt wegwerfen 
  var pBestDropObj, iBestDropPrio=1000; 
  var pObj, iPrio; 
  var idCollectObj = GetID(pCollectObj); 
  for(var i = 0; pObj = Contents(i); i++) 
    if ((iPrio = GetContentsPriority(pObj, iArrowPriority, idCollectObj)) < iBestDropPrio) 
      { 
      iBestDropPrio = iPrio; 
      pBestDropObj = pObj; 
      } 
  // Dieses Objekt zurückgeben 
  return pBestDropObj; 
  } 

private func GetContentsPriority(pContents, iArrowPriority, idCollectObj) 
  { 
  // Pfeil? Normalerweise hohe Priorität, wenn nicht gerade ein anderes Pfeilpaket eingesammelt werden soll 
  if (pContents->~IsArrow()) return 302+iArrowPriority; 
  // Pfeilpakete sind etwas geringer brauchbar, damit sie eher abgelegt werden (geht schneller...) 
  if (pContents->~IsArrowPack()) return 301+iArrowPriority; 
  // Objekt ist eine Handwaffe? 
  var idCnt = GetID(pContents); 
  if (pContents->~IsFightWeapon(this))
    { 
    // Es soll eine Handwaffe eingesammelt werden 
    if (DefinitionCall(idCollectObj, "IsFightWeapon") || idCnt==SPER) 
      // Hat schon eine Handwaffe 
      if (HasFightWeapon()) 
        // Sehr niedrige Priorität 
        return 1; 
    // Ansonsten relativ hohe Priorität 
    return 220; 
    } 
  // Bogen? 
  if (idCnt==BOW1) 
    { 
    // Noch ein Bogen vorhanden? Muss nicht sein 
    if (idCollectObj==BOW1 || ContentsCount(BOW1) > 1) return 1; 
    // Ansonsten fast so wichtig wie die Handwaffe (für die KI...) 
    return 216; 
    } 
  // Der Rest kann kaum gebraucht werden; ablegen nach Wert 
  return GetValue(pContents); 
  } 
 
public func ReadyToFire() 
{ 
  // Nur beim Laufen - und neu auch beim Reiten... 
  if (WildcardMatch(GetAction(),"*Walk*") || GetAction() == "Ride" || GetAction() == "RideStill") return 1; 
  return 0; 
} 

/* Während des Kämpfens */ 
private func Fighting(pEnemy) 
{ 
  // Lanze verlieren 
  var pLance; 
  if (pLance = HasLance())  
    pLance->LNCA::Unbuckle();   
  // Kampf mit Handwaffe 
  var pWeapon; 
  if (pWeapon = GetFightWeapon()) 
  { 
    // Erstmal Kampfaktion setzen 
    if (GetAction() != pWeapon->~GetFightAction()) 
      return (SetAction(pWeapon->~GetFightAction(), pEnemy)); 
    // Je nach Waffengeschwindigkeit Schlag durchführen 
    if (!Random(pWeapon->~GetStrikeTime())) 
      return (SetAction(pWeapon->~GetStrikeAction())); 
    // Weiterkämpfen 
    return 1; 
  } 
  // Keine Waffe: Gegner schlagen 
  if (!Random(4))  
    SetAction("Punch"); 
  // Weiterkämpfen 
  return 1; 
} 

protected func Death(int iKiller, a,b,c,d,e)
{
  
  // Basisfunktion aufrufen
  return _inherited(iKiller, a,b,c,d,e) ;
}
 
/* Schlag mit Handwaffe */ 
private func Striking() 
{ 
  // Waffe bestimmen 
  var pWeapon; 
  if (!(pWeapon = GetFightWeapon())) return 0; 
  // Sound 
  pWeapon->~StrikeSound(); 
  // Kein Schaden 
  if (!Random(2)) return 1; 
  // Schaden ausrechnen 
  var pEnemy = GetActionTarget(); 
  var iDamage = pWeapon->~GetStrikeDamage(); 
  iDamage = iDamage * (100 - pEnemy->~IsShielded()) / 100; 
  // Schlagen! 
  Punch(pEnemy, iDamage); 
  return 1; 
} 
 
protected func QueryCatchBlow(pObj) 
{ 
  // Schutzfaktor: lässt Objekte zurückprallen 
  var iPower = BoundBy(GetMass(pObj), 0, 50) * Distance(GetXDir(pObj), GetYDir(pObj)) * Distance(GetXDir(pObj), GetYDir(pObj));   
  
  var iExtra=0;
  if(WildcardMatch(GetAction(),"*KneelShield*"))
  {
   if(GetDir() == DIR_Left)
    if(GetX(pObj) < GetX()+7)iExtra=50;
   
   if(GetDir() == DIR_Right)
    if(GetX(pObj) > GetX()-7)iExtra=50;
  }
  
  if(Random(50000) + iPower / 3 < (IsShielded()+iExtra) * 1000) 
  {           
    // Projektil abprallen lassen 
    if(GetXDir(pObj) < 0) if(GetX(pObj) < GetX() + 6) SetPosition(GetX() + 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    if(GetXDir(pObj) > 0) if(GetX(pObj) > GetX() - 6) SetPosition(GetX() - 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    SetXDir(BoundBy(-GetXDir(pObj) / 3, -10, 10), pObj); SetYDir(0-(iExtra/5), pObj); 
    // Sounds dazu 
    if(GetMass(pObj) >= 10 && HasArmor()) Sound("ClonkHit*"); 
    if(GetMass(pObj) < 10 && HasArmor()) Sound("ArrowRicochet*"); 
    ProtectedCall(pObj,"Hit");       
    return 1; 
  }            
  return _inherited(pObj); 
} 
 
protected func CatchBlow(iLevel, pObj) 
{ 
  // Zusätzlicher Schutzfaktor: Weniger Energieverlusst 
  DoEnergy(-iLevel * IsShielded() / 100); 
  return _inherited(iLevel * (100 - IsShielded()) / 100, pObj);     
} 
 
private func Scaling() 
  { 
  var szDesiredAction; 
  if (GetYDir()>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale"; 
  if (GetAction() != szDesiredAction) SetAction(szDesiredAction); 
  return 1;    
  } 

/* Aufwertung: Kann sich nicht zu einem Ritter aufwerten */

public func IsRevaluationBlocked(object target) { return target->~IsKnight(); }

public func OnRevaluation()
  {
  // Der Magier bekommt bei Aufwertung Mana - Der Kanderianer also Waffen
  var i = Random(2);
       if (!i--) { CreateContents(SHI2); CreateContents(ARWP); }
  else if (!i--) CreateContents(SPER);
  }

/*
protected func Walking()
{
 var desAction;
 if(FindContents(SPER))
  {
   desAction="WalkSpear";
  }
 else
  desAction="Walk";
  
 if(GetAction() != desAction)
  SetAction(desAction);
}*/

/* Um evtl einen Speer anzuzeigen */

protected func Ejection() { CheckArmed(); }
protected func Departure() { CheckArmed(); }

private func CheckArmed()           // Bewaffnung (mit Speer) prüfen
{
  if (GetAction()=="Walk") if(FindContents(SPER)) return SetAction("SpearWalk");
  if (GetAction()=="Jump") if(FindContents(SPER)) return SetAction("SpearJump");
  if (GetAction()=="KneelDown") if(FindContents(SPER)) return SetAction("SpearKneelDown");
  if (GetAction()=="SpearWalk") if(!FindContents(SPER)) return SetAction("Walk");
  if (GetAction()=="SpearJump") if(!FindContents(SPER)) return SetAction("Jump");
  if (GetAction()=="SpearKneelDown") if(!FindContents(SPER)) return SetAction("KneelDown");
}

/* Aufnahme */
protected func Collection(pObj)
{
  CheckArmed();
  return _inherited(pObj);
}

protected func StartKneelSpear()
{
 if(!GetEffect("SpearKneeling",this))
  AddEffect("SpearKneeling",this,1,2,this);
}

protected func AbortKneelSpear()
{
 if(GetEffect("SpearKneeling",this))
  RemoveEffect("SpearKneeling",this);
}

func FxSpearKneelingStart(pTarget,iEffectNumber)
{

}

func FxSpearKneelingTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime % 10 == 0)
  if(GetAction() != "KneelShieldSpear")return -1;
 var fac=1;
 if(GetDir() == DIR_Left)fac=-1;
 var x=Cos(GetPhase()*10,15)*fac;
 var y=Sin(GetPhase()*10,15)*-1;

 
 for(var obj in FindObjects(Find_AtPoint(x,y),Find_OCF(OCF_Alive)))
 {
  var val=Sqrt(GetXDir(obj)**2+GetYDir(obj)**2);
  if(val < 25)continue;
  
  if(GetDir(pTarget) == DIR_Left)
   {
    if(GetXDir(obj) < 0)continue;
   }
  else
   {
    if(GetXDir(obj) > 0)continue;
   }
  
  Punch(obj,val/3);
 }
}


