/*-- Meuchelmörder --*/

#strict 2

#include KNIG

local fHide;

protected func Recruitment()
{
  // Doppelschaden Effect
  if(!GetEffect("DoubleDamage", this)) AddEffect("DoubleDamage", this, 1,0,this);
  _inherited();
}

func FxDoubleDamageDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  return iDmg*2;
}

//! Welche Objekte kann dieser Clonk einsetzen?
public func CanUse(idObj)
{
  // Wurfmesser
  if (idObj == KNFP) return 1;
  // Keule
  if (idObj == CLUB) return 1;
  // Armbrust
  if (idObj == CRBW) return 1;  
  // Unbekannt
  return 0; 
}

//! Bevorzugte Waffentypen für selbständige Bewaffnung
public func GetFightWeaponType(index) 
{ 
  if (index == 0) return KNFP; 
  if (index == 1) return CLUB; 
  return; 
}

/* Armbrust ========================================================== */ 
 
private func AimBowToTarget(iXOffset, iYOffset) 
{ 
  // Richtung 
  if (iXOffset > 0) SetDir(DIR_Right); 
  if (iXOffset < 0) SetDir(DIR_Left); 
  // Zielrichtung 
  var iAngle = Angle(0,0,Abs(iXOffset),iYOffset); 
  SetPhase(BoundBy( iAngle / 23, 0, 5)); 
  // Höher zielen für größere Distanz 
  if (GetPhase() > 1) 
    SetPhase(BoundBy(GetPhase() - Abs(iXOffset) / 150, 1, 5)); 
  // Fertig 
  return 1; 
} 
 
private func AimBowToAngle(iAngle) 
  { 
  // Winkel anpassen 
  while (iAngle > 180) iAngle -= 360; 
  // Richtung 
  if (iAngle > 0 ) SetDir(DIR_Right); else SetDir(DIR_Left); 
  // Zielrichtung 
  SetPhase(BoundBy( (Abs(iAngle)+8)/16, 0,7)); 
  // Fertig 
  return 1; 
  } 

private func SimBow(int iAngle, int iTargetX)
{
  // Startwerte
  var iX, iY, iR, iXDir, iYDir, iRDir;
  GetBowOut(0, iAngle, false, iX, iY, iR, iXDir, iYDir, iRDir);
  // Koordinaten umrechnen
  iX += GetX(); iY += GetY();
  // Zeit, die das Geschoss fliegen muss
  var iTime = (iTargetX - iX) * 10 / iXDir;
  // Simulieren
  return SimFlight(iX, iY, iXDir, iYDir, 1, 100, iTime);
}

private func FireCrossbowAt(int iX, int iY, bool fForce) 
{ 
  var v = 10, // 10 px / tick
      f = 200, // Fixpunktfaktor
      g = -GetGravity() * f * f / 1000 / v, // 0,2 px / tick²
      x = (iX - GetX()) * f / v,
      y = -(iY - GetY()) * f / v /* Korrektur: */ - Abs(x) * GetGravity() / 2000,
      d = y * y + x * x,
      k = y + f * f * f / 2 / g,
      w = k * k - d;
  // Außer Reichweite?
  if(w < 0 && !fForce) return;
  // Flugzeiten und Winkel berechnen
  var t1 = Sqrt( (k + Sqrt(w)) * (f * f * f / g) ),
      t2 = Sqrt( (k - Sqrt(w)) * (f * f * f / g) ),
      phi1 = ArcCos(x, t1),
      phi2 = ArcCos(x, t2);
  if(y < g * t1 / f * t1 / f / f) phi1 = -phi1;
  if(y < g * t2 / f * t2 / f / f) phi2 = -phi2;
    // Winkel umrechnen
    phi1 = (270 - phi1) % 360 - 180; phi2 = 90 - phi2;
  // Erste Lösung außerhalb der Grenzen?
    if(!Inside(phi1, -120, 120) && !fForce) return;
    phi1=BoundBy(phi1, -120, 120);
    // Zielwinkel probieren
  var iAngle;
  if(!SimBow(phi1, iX))
    iAngle = phi1;
  // Zweite Schusslösung?
  else if(t2 < t1 * 3 && !SimBow(phi2, iX))
    iAngle = phi2;
  else if(fForce)
    iAngle = phi1;      
    else
    return;
  // Zielt gar nicht: erst mal laden 
  if (!IsAiming())
    {
    LoadCrossbow(FindContents(CRBW), 1);  
    return;  
    }
  // Zielen 
  AimBowToAngle(iAngle); 
  // Feuern 
  return Contents()->~FireAngle(iAngle);
} 

/* Beginnt den Zielvorgang */  
public func AimCrossbow() 
{ 
  // Gibt den Rückgabewert von SetAction zurück 
  if (IsRiding()) 
    return SetAction("RideAimCrossbow");
  else 
    return SetAction("AimCrossbow");
}  
  
/* Beginnt den Ladevorgang */  
public func LoadCrossbow(pBow, fFirstTime) 
  { 
  // Nur mit Pfeil! 
  if (!GetBoltCount() && !FindObject(HOOK)) 
    { 
    // Kein Pfeil fürs erste Zielen: Meldung ausgeben 
    if (fFirstTime) 
      { 
      Sound("KnightFailure"); 
      PlayerMessage(GetController(), "$MsgNoArrow$", this); // Kein Pfeil 
      } 
    return 1; 
    } 
  // Bogen lieber wieder auswählen (GetArrow wählt gerne ab...) 
  ShiftContents(0,0,CRBW); 
  // Schon beim Nachladen? 
  if (IsLoading()) return 1;
   Sound("CrossbowReload*"); 
  // War gerade beim Zielen: letzten Winkel sichern 
  if (IsAiming())  
    iLastAimAngle = GetPhase();  
  else  
    iLastAimAngle = 4; 
  // Muss den Rückgabewert von SetAction zurückgeben, da das Setzen durch Überladungen fehlgeschlagen sein kann 
  if (IsRiding()) 
    return SetAction("RideLoadCrossbow");
  else 
    return SetAction("LoadCrossbow");
  }
  
protected func LoadingCrossbow()
{
  // Mittleren Teil der Animation mehrfach abspielen
  if(GetPhase()==7)
  {
    if(Local()++<2)
      SetPhase(2);
    else Local() = 0;
  }
}
 
protected func BeginAiming() { Contents()->~ActualizePhase(this); return SetPhase(iLastAimAngle); } 

/* Winkel, in den der Ritter momentan zielt */
public func GetBowAimAngle()
  { 
  // Richtung ermitteln 
  var iOutDir; if (!GetDir()) iOutDir=-1; else iOutDir=1; 
  // Winkel (von oben) ermitteln
  return iOutDir*GetPhase()*115/7;
  }

/* Zielabweichung des Ritters. Überladbar. */
public func GetAimingDeviation(pBow)
  {
  return 5;
  }
  
/* Ausgangsgeschwindigkeit und -position, wenn der Ritter mit dem Bogen in angegebene Richtung schießt */
public func GetCrossbowOut(pBow, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
  {
  // Zufallsabweichung
  if(fDeviate) 
    { 
    var iDeviation = GetAimingDeviation(pBow)/2;
    iAngle += Random(iDeviation * 2 + 1) - iDeviation;
    }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 150);
  iOutYDir = -Cos(iAngle, 150);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return 1;
  } 

private func ChangeBowAimAngle(iChange) 
  { 
  var iOldAngle; 
  // Derzeitigen Winkel ermitteln 
  if (IsAiming()) iOldAngle = GetPhase(); 
  if (IsLoading()) iOldAngle = iLastAimAngle; 
  // Am Anschlag? 
  if (!Inside(iOldAngle + iChange, 0, 7)) return; 
  // Neuen Winkel setzen 
  if (IsAiming()) SetPhase(iOldAngle + iChange); 
  if (IsLoading()) iLastAimAngle += iChange; 
  } 
 
public func BowControlUp(pBow, &rVal) 
  { 
  ChangeBowAimAngle(-1); 
  // Funktion überladen 
  return 1; 
  } 
 
public func BowControlDown(pBow, &rVal) 
  { 
  ChangeBowAimAngle(+1); 
  // Funktion überladen 
  return 1; 
  }

//! Zielt der Clonk gerade mit dem Bogen? 
private func IsAiming() 
{ 
  return (GetAction() == "AimBow" || GetAction() == "AimCrossbow" || GetAction() == "RideAimBow"); 
} 
 
//! Lädt der Clonk gerade den Bogen nach? 
private func IsLoading() 
{ 
  return (GetAction() == "LoadBow" || GetAction() == "LoadCrossbow" || GetAction() == "RideLoadBow"); 
}

/* Bolzen aus dem Inventar nehmen */ 
public func GetBolt(idID)
  { 
  // Einzelne Bolzen suchen 
  var pObj, pBolt; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsBolt() || pObj->~IsHook())
      // ID-Vergleich
      if(!idID || GetID(pObj)==idID)
        return pObj;
  // Bei Bolzen Pakete aufteilen 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsBoltPack())
     // ID-Vergleich 
     if(!idID || pObj->~UnpackTo()==idID)
      // Bolzen aus Paket verwenden
      if(pBolt = pObj->~GetItem()) return pBolt;
  // Keine Bolzen gefunden 
  return 0; 
  }

/* Bolzen im Inventar zählen */ 
private func GetBoltCount() 
{
   return GetSpecialCount("IsBolt");
}

/* Zielaktion abbrechen */ 
public func StopAiming() 
{ 
  // Zielt gar nicht: nix machen 
  if (!IsAiming() && !IsLoading()) return 0; 
  // Stop 
  SetComDir(COMD_Stop); 
  // Basisaktion setzen 
  if (IsRiding()) 
    SetAction("Ride"); 
  else 
    SetAction("Walk"); 
  // Sound 
  Sound("Grab");
  // Eventuell gleich Seilende auswählen
  if(FindContents(RPND))
    ShiftContents(0, 0, RPND); 
  return 1; 
} 

/* Überprüft ob der übergebene Typ ein Pfeil ist  */ 
private func CheckBolt(idDef) { return idDef->~IsBolt(); } 
 
/* Überprüft ob der übergebene Typ ein Pfeilpaket ist */ 
private func CheckBoltPack(idDef) { return idDef->~IsBoltPack(); } 

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Der Meuchelmörder kann zusätzlich noch 30 Bolzen und 10 Pfeile tragen
  if(iIndex == 0) { if(fAmount) return 10; return "IsArrow"; }
  if(iIndex == 1) { if(fAmount) return 30; return "IsBolt"; }
}

//! Trägt der Clonk eine Armbrust und hat Bolzen dabei? 
private func IsCrossbowEquipped() 
{ 
  // Armbrust dabei? 
  if (!FindContents(CRBW)) return 0; 
  // Bolzen dabei? 
  if (!GetBoltCount()) return 0; 
  // Armbrust und Bolzen dabei 
  return 1; 
} 

private func ExecuteAttackCrossbow(pTarget) 
  { 
  // Zielposition ermitteln 
  var iTx = GetX(pTarget), iTy = GetY(pTarget); 
  // Ziel in Reichweite? 
  if (Abs(iTx-GetX())>300 || Abs(iTy-GetY())>300) 
    // Nicht? Mögliche Abschußposition einnehmen 
    return SetCommand(this,"MoveTo",0, BoundBy(GetX(),iTx-290,iTx+290),GetY()); 
  // Armbrust auswählen 
  ShiftContents(0, 0, CRBW); 
  // Kann leider von hier aus nicht zielen 
  if (!IsAiming()) 
    if (!ReadyToFire()) 
      return 0; 
  // Ziel nicht im Freien 
  if (Contained(pTarget)) 
    return 0; 
  // Ziel von hier aus nicht in Sicht 
  if (!PathFree( GetX(),GetY(), iTx, iTy )) 
    if (!PathFree( GetX(),GetY()-20, iTy-20 )) 
      return 0; 
  // Schießen 
  FireCrossbowAt(iTx,iTy, false); 
  // Weitermachen   
  return 1; 
  } 

protected func ControlLeftDouble()
{
  if(GetAction() == "Crack")
  { SetAction("Walk"); SetComDir(COMD_Left); return 1; }
   
  return _inherited(Par(),Par(1),Par(2));
}
 
protected func ControlRightDouble()
{
  if(GetAction() == "Crack")
  { SetAction("Walk"); SetComDir(COMD_Right); return 1; }
  
 return _inherited(Par(),Par(1),Par(2));
}

/* Spezialtaste: Inventarwechsel */ 
protected func ControlSpecial(pByObject) 
{ 
  [$CtrlInventoryDesc$|Image=CXIV]
  if(!pByObject) pByObject = this;
  // an Inventar weitergeben
  if ( Control2Contents("ControlSpecial", pByObject) ) return 1; 
  return (_inherited(pByObject)); 
} 

protected func Fighting()
{
  // Enttarnen beim Kämpfen
  RemoveEffect("Hide", this);
  return _inherited();
}


// Callback zum Kurzzeitigen Sichtbarmachen
public func DoShow(iAmount)
{
  var i;
  if(i = GetEffect("Hide", this))
  {
    EffectVar(3, this, i) = BoundBy(EffectVar(3, this, i)-iAmount, 0, 235);
    SetClrModulation(RGBa(255,255,255,EffectVar(3, this, i)));
  }
}

// Den Clonk tarnen
public func Hide(fNoEnemyCheck, fInstantHide, fNoMessage)
{
  // Schon einen Tarneffekt? Dann nichts machen
  if(GetEffect("Hide", this))
  {
    if(fInstantHide)
    {
      EffectVar(3, this, GetEffect("Hide", this)) = 234;
      FxHideTimer(this, GetEffect("Hide", this));
    }
    return;
  }
  // Suche nach Gegnern in der Nähe
  if(!fNoEnemyCheck)
    if(FindObject2(Find_Hostile(GetOwner()), Find_OCF(OCF_CrewMember), Find_Container(Contained()), Find_Distance(150)))
    {
      if(fNoMessage) return;
      return PlayerMessage(GetOwner(), "$MsgEnemyToNear$", this);
    }
  // Tarneffekt starten
  AddEffect("Hide", this, 1, 8, this, 0, fInstantHide);
  return 1; 
}

// Den Clonk entarnen
public func UnHide()
{
  RemoveEffect("Hide", this);
  return 1;
}

// Effektvariablen
// 0: Gespeicherter Alphawert
// 1: Hilfspobejekt zum anzeigen der Partikel
// 2: Winkel für die Partikel
// 3: aktueller Alphawert
// 4: Gespeicherte Kategorie
func FxHideStart(pTarget, iNumber, fTmp, fInstantHide)
{
  // Alpha auf 240 setzen
  var iAlpha = GetRGBaValue(GetClrModulation(), 0);
  EffectVar(0, pTarget, iNumber) = iAlpha;
  EffectVar(3, pTarget, iNumber) = iAlpha;
  // Instanttarnen?
  if(fInstantHide)
    EffectVar(3, pTarget, iNumber) = 234;
  // Walk Physical verringern
  SetPhysical("Walk",GetPhysical("Walk", 0,pTarget)-20000,PHYS_StackTemporary,pTarget);
  SetPhysical("Scale",GetPhysical("Scale", 0,pTarget)-20000,PHYS_StackTemporary,pTarget);
  SetPhysical("Hangle",GetPhysical("Hangle", 0,pTarget)-20000,PHYS_StackTemporary,pTarget);
  // Hilfsobjekt erzeugen, dass den Clonk für den Spieler anzeigt
  var pShower = CreateObject(SNHP, 0, 0, GetOwner());
  SetCategory(8388609, pShower);
  EffectVar(1, pTarget, iNumber) = pShower;
  pShower->SetAction("Attach", this);
  // Kategorie auf C4D_MouseIgnore setzten
  EffectVar(4, pTarget, iNumber) = GetCategory();
  SetCategory(GetCategory()|C4D_MouseIgnore);
  // Flag setzen
  fHide = 1;
}

func FxHideTimer(pTarget, iNumber)
{
  // Transparenter werden
  if(EffectVar(3, pTarget, iNumber)<235)
  { 
    var iChange = Min(235-EffectVar(3, pTarget, iNumber), 20);
    EffectVar(3, pTarget, iNumber) += iChange;
    SetClrModulation(RGBa(255,255,255,EffectVar(3, pTarget, iNumber)));
  }
  // Partikeleffekt
  EffectVar(2, pTarget, iNumber) += 40;
  if(EffectVar(2, pTarget, iNumber)==360) EffectVar(2, pTarget, iNumber) = 0;
  EffectVar(1, pTarget, iNumber)->CreateParticle("HideSpark", Sin(EffectVar(2, pTarget, iNumber), 20), Cos(EffectVar(2, pTarget, iNumber), 20),-Sin(EffectVar(2, pTarget, iNumber), 2),-Cos(EffectVar(2, pTarget, iNumber), 2), 30, GetColorDw(), EffectVar(1, pTarget, iNumber));
}

func FxHideDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  // Heilen nichts machen
  if(iDmg > 0) return iDmg;
  // aber bei Schaden wird die Tarnung aufgegeben
  RemoveEffect("Hide", pTarget);
  return iDmg;
}

func FxHideStop(pTarget, iNumber, fTmp)
{
  // Alpha auf alten Wert setzen
  var iAlpha = GetRGBaValue(GetClrModulation(), 0);
  SetClrModulation(DoRGBaValue(GetClrModulation(), EffectVar(0, pTarget, iNumber)-iAlpha, 0));
  // Walk zurücksetzten
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Energy");
  SetPhysical("Energy", GetPhysical("Energy", 1, pTarget)/2, 2, pTarget);
  // Hilfsobjekt entfernen
  RemoveObject(EffectVar(1, pTarget, iNumber));
  // Kategorie zurücksetzten
  SetCategory(EffectVar(4, pTarget, iNumber));
  fHide = 0;
}

public func IsNotHidden() { return !fHide; }
public func IsHidden() { return fHide; }

public func IsAssassin() { return true; }

public func Assassasinated()
{
  var pTarget = GetActionTarget();
	if(pTarget && ObjectDistance(pTarget)<22 && pTarget != this) // mit extra Sicherheit dass sich der Assassine nicht selbst ersticht
  {
    if(pTarget->~HasArmor())
    {
      Sound("ClonkHit*");
      DoEnergy(-30, pTarget);
    }
    else
      pTarget->Kill();
  }
}

public func Stuned()
{
  var pTarget = GetActionTarget();
  if(pTarget && ObjectDistance(pTarget)<22)
  {
    if(pTarget->~HasArmor())
    {
      Sound("ClonkHit*");
      AddEffect("SleepNSpell",pTarget,123,3,0,GetID(),37*((115-GetEnergy(pTarget)*100000/GetPhysical("Energy",0, pTarget))/5));
    }
    else
      AddEffect("SleepNSpell",pTarget,123,3,0,GetID());
  }
}

protected func FxSleepNSpellStart(object pTarget, int iEffectNumber, int iTemp, int iTime) {
  // temporäre aufrufe
  if(iTemp) return;
  // Scheintoter Zustand
  ObjectSetAction(pTarget,"Dead");
  // Zeit bis zum Aufwachen speichern
  EffectVar(0,pTarget,iEffectNumber)=iTime;
  EffectVar(1,pTarget,iEffectNumber) = LocalN("ai_iRange", pTarget);
  LocalN("ai_iRange", pTarget) = 0;
  // Fertig
  return 1;
}

protected func FxSleepNSpellEffect(szNewEffect, pTarget, iEffectNumber)
  {
  // Wenn schon ein Schlafzauber wirkt, darf kein neuer dazukommen
  if (szNewEffect == "SleepNSpell") return -1;
  // Sonst keine Behandlung
  return;
  }

protected func FxSleepNSpellStop(object pTarget, int iEffectNumber) {
  // clonk hat den Schlaf nicht überstanden ;)
  if(!GetAlive(pTarget)) return 1;
  // aufstehen
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  // stehenbleiben
  SetComDir(COMD_Stop,pTarget);
  // Markereffekt entfernen (für KI benötigt)
  RemoveEffect("IntFoundMarker", pTarget);
  LocalN("ai_iRange", pTarget) = EffectVar(1,pTarget,iEffectNumber);
  // Fertig
  return 1;
}

protected func FxSleepNSpellTimer(object pTarget, int iEffectNumber, int iTime) {
  var iMaxTime=EffectVar(0,pTarget,iEffectNumber);
  // nach einer bestimmten Zeit aufhören
  if(iMaxTime && iTime>=iMaxTime) return(-1);
  // Sternchen-Partikel casten
  CreateParticle("MaterialSpark",GetX(pTarget)+Sin(iTime*2, 4)-5+10*GetDir(pTarget),GetY(pTarget),0,0,20,RGB(155,155));
  CreateParticle("MaterialSpark",GetX(pTarget)+Sin(iTime*2+130, 4)-5+10*GetDir(pTarget),GetY(pTarget),0,0,20,RGB(155,155));
  CreateParticle("MaterialSpark",GetX(pTarget)+Sin(iTime*2+260, 4)-5+10*GetDir(pTarget),GetY(pTarget),0,0,20,RGB(155,155));
}

protected func FxSleepNSpellDamage(object pTarget, int iEffectNumber, int iDmgEngy) {
  // Bei Damage aufwachen
  RemoveEffect(0,pTarget,iEffectNumber);
  return iDmgEngy;
}

public func BeginLoading()
{
  AddEffect("IntProgress", this, 1, 1, this, 0, 80);
}

func FxIntProgressStart(pTarget, iNumber, fTmp, iMaxTime)
{
  if(fTmp) return;
  // Aktion speichern
  EffectVar(0, pTarget, iNumber) = GetAction();
  // Hilfsobjekt erzeugen, dass den Fortschritt anzeigt
  var pShower = CreateObject(SNHP, 0, 0, GetOwner());
  SetCategory(8388609, pShower);
  EffectVar(1, pTarget, iNumber) = pShower;
  pShower->SetAction("Attach", this);
  // Zeit speichern
  EffectVar(2, pTarget, iNumber) = iMaxTime;
}

func FxIntProgressTimer(pTarget, iNumber, iTime)
{
  if(GetAction() != EffectVar(0, pTarget, iNumber)) return -1;
  if(EffectVar(3, pTarget, iNumber)/12 > iTime*30/EffectVar(2, pTarget, iNumber)) return;
  EffectVar(3, pTarget, iNumber) += 12;
  var iColor = RGB(255-255*iTime/EffectVar(2, pTarget, iNumber), 255*iTime/EffectVar(2, pTarget, iNumber));
  EffectVar(1, pTarget, iNumber)->CreateParticle("NoGravSpark", Sin(EffectVar(3, pTarget, iNumber), 15), Cos(EffectVar(3, pTarget, iNumber), 15),0,0, 30, iColor, EffectVar(1, pTarget, iNumber));
}

func FxIntProgressStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return;
  // Hilfsobjekt entfernen
  RemoveObject(EffectVar(1, pTarget, iNumber));
}

protected func Cracking()
{
  if(!FindDoor()) return SetAction("Walk");
}

protected func Cracked()
{
  if(GetActTime()<216) return;
  SetAction("Walk");
  var pObj = FindDoor();
  if(!pObj) return;
  pObj->~Open();
}

private func FindDoor()
{
  var pObj;
  if(pObj = FindObject2(Find_Or(Find_ID(CPGL), Find_ID(CPGR)), Find_Distance(14)))
    if(GetComDir(pObj)==COMD_Down && !GetYDir()) return pObj;
}

// Kontext

public func ContextHide()
{
  [$CtxHideDesc$|Image=CMFG|Condition=IsNotHidden]
  // Tarnen
  Hide();
  return 1;
}

public func ContextUnHide()
{
  [$CtxUnHideDesc$|Image=CMFG|Condition=IsHidden]
  // Enttarnen
  UnHide();
  return 1;
}

public func ContextCrackLock()
{
  [$CtxCrackLockDesc$|Image=CPGR|Condition=FindDoor]
  SetAction("Crack");
  AddEffect("IntProgress", this, 1, 1, this, 0, 216);
  return 1;
}
