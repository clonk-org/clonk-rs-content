/*-- Skelett --*/

#strict 2

#include CLNK

protected func Initialize()
{
  // Wassereffekt anhängen
  AddEffect("SkeletonWaterCheck",this,20,1,this);
  return _inherited();
}

protected func Recruitment()
{
  // Portrait setzen
  SetPortrait("1");
  DoEnergy(100);
  return _inherited();
}

// Zur Sicherheit, damit Überladungen keinen Fehler beim Starten geben
protected func Scaling()
{
  var szDesiredAction;
  if (GetYDir()>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (GetAction() != szDesiredAction) SetAction(szDesiredAction);
  return 1;   
}

protected func ContactBottom()
{
  // Skelette können am Grund eines Sees laufen
  if (InLiquid())
    SetAction("Walk");
  return 1;
}

protected func Death()
{
  // Ein Skelett zerfällt zu Knochen
  CastObjects(BONE,4,10);
 
  _inherited(Par(),Par(1),Par(2));
 
  if(this)
    RemoveObject(0,1);
  return 1;
}

/* Essen */  

public func Feed(iLevel)
{
  // Schon mal ein Skelett essen sehen?
  CastParticles ("DoughSlice", Random(3)+4, 13, 0,0, 30,35, RGB(170,115,60), RGB(145,95,50));
  Sound("WoodHit*");Sound("WoodHit*");
  return 1;
}

protected func FxSkeletonWaterCheckTimer(pTarget,iEffectNumber)
{
  // Im Wasser beim Springen...
  if(!InLiquid()) return 1;
  if(GetAction() != "Jump" && GetAction() != "Tumble") return 1;
  if(!GetXDir() && !GetYDir()) return 1;
  
  // ... wird das Skelett leicht abgebremst
  var xDir = GetXDir();
  if(GetXDir() > 16) xDir=(GetXDir()*7)/8;
  var yDir = GetYDir();
  if(GetYDir() > 16) yDir=(GetYDir()*7)/8;
  SetXDir(xDir);
  SetYDir(yDir);
  
  return 1;
}

// Böser Effektmissbrauch! :O
protected func FxSkeletonWaterCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  // Feuer ist einfach immer böse
  if(iCause == 35) return iDmg;
  // Explosionen auch!
  if(iCause == 1)  return iDmg;
  // Heilen sowiesoüberhauptundeh!
  if(iDmg > 0)return iDmg;
 
  // Im Nahkampf nur etwas abschwächen
  if(iCause == 40) return (iDmg/10)*9;
 
  return (iDmg/10)*6;
}

public func ControlDown()
{
  // Skelett kann sich "tot" stellen
  if(GetPlrDownDouble(GetOwner()) && GetAction() == "Walk") SetAction("Dead");
}

public func ControlUp()
{
  // Nach dem "Tot-Stellen" wieder aufstehen
  if(GetAction() == "Dead") SetAction("UnDead");
}

public func ControlSpecial()
{
  // Inventarwechsel
  ShiftContents(this);
}

public func ControlThrow()
{
 // Ablegen ist erlaubt.
 if(GetPlrDownDouble(GetOwner())) return false;
 if(ContentsCount()) if(Contents()->~ControlThrow()) return true;
 // Noch Platz für andres oder so
 else return false;
}

/* Itemlimit */
public func MaxContentsCount() { return 2; }

// Schlagen mit dem Schwert, der Axt und Werfen des Speeres

public func ReadyToFire() 
{ 
  // Nur beim Laufen - und neu auch beim Reiten... 
  if (GetAction() == "Walk" || GetAction() == "Ride" || GetAction() == "RideStill") return 1; 
  return 0; 
} 

//! Welche Objekte kann dieser Clonk einsetzen?
public func CanUse(idObj)
{
  // Schwert
  if (idObj == SWOR) return 1;
  // Axt
  if (idObj == AXE1) return 1;  
  // Speer 
  if (idObj == SPER) return 1;   
  // Unbekannt 
  return 0; 
} 
 
//! Bevorzugte Waffentypen für selbständige Bewaffnung (zur Zeit werden genau zwei Waffentypen erwartet). 
public func GetFightWeaponType(index) 
{ 
  if (index == 0) return SWOR; 
  if (index == 1) return AXE1; 
  return 0; 
} 

//! Prüft ob der Clonk eine Handwaffe trägt, die er verwenden kann. 
public func HasFightWeapon(pClonk) 
{  
  // Anderer Clonk 
  if (pClonk)  
    return (pClonk->~HasFightWeapon()); 
  // Dieser Clonk 
  var pObj; 
  for (var i = 0; pObj = Contents(i); i++) 
    if (pObj->~IsFightWeapon() && CanUse(GetID(pObj))) 
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
    if (pObj->~IsFightWeapon() && CanUse(GetID(pObj))) 
      return pObj; 
  // Keine Waffe 
  return 0; 
} 

private func Fighting(pEnemy) 
{ 
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

private func ExitSpear() 
{ 
  return Throwing(FindSpearContents()); 
} 

private func Throwing(pObj) 
{            
  // Ggf. zu werfendes Objekt suchen 
  if (!pObj) 
  { 
    pObj = Contents(0); 
    if (!Contents()) return 0; 
  } 
  // Wurfparameter berechnen 
  var iX, iY, iR, iXDir, iYDir, iRDir; 
  if(GetID(pObj) != SPER && !pObj->~IsSpear()) 
  { 
    // Standardwurf   
    iX = 0; if (!GetDir()) iX = -iX; 
    iY = -10; 
    iR = Random(360); 
    iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir; 
    iYDir = -GetPhysical("Throw") / 25000; 
    iRDir = Random(40) - 20; 
  } 
  else 
  { 
    // Speerwurf 
    iX = 10; if (!GetDir()) iX = -iX; 
    iY = -6; 
    iR = 150; if(!GetDir()) iR = 180 - iR; 
    iXDir = 5; if(!GetDir()) iXDir = -iXDir; 
    iYDir = -2; 
    iRDir = 4; if(!GetDir()) iRDir = -iRDir; 
  } 
  // Reitet? Eigengeschwindigkeit addieren 
  if (GetActionTarget()) if (IsRiding()) 
  {
    iXDir += GetXDir(GetActionTarget()) / 10;
    // Steigungen beim Gehen werden leider nicht in die Y-Dir eingerechnet. Bei Steigungen
    // kann es daher vorkommen, dass der Wurf den Reiter vom Pferd haut. Was dagegen tun..?
    iYDir += GetYDir(GetActionTarget()) / 10; 
  } 
  // Werfen! 
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir); 
  return 1;   
} 

/* Speer im Inventar finden */ 
private func FindSpearContents() 
  { 
  var pObj,i; 
  // SPER suchen 
  if (pObj = FindContents(SPER)) return (pObj); 
  // Objekte mit IsSpear() suchen 
  while (pObj=Contents(i++)) if (pObj->~IsSpear()) return (pObj); 
  // Nichts gefunden 
  return; 
  } 
