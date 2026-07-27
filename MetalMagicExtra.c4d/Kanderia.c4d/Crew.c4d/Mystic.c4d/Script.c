/*-- Mystiker --*/

#strict 2

#include MCLK

local pMagicObject;
local iObjectsMask;

local fGrabbedInfo;

// Der Mystiker benutzt immer die InfoSektion eines Maguses
protected func Recruitment()
{
  if(!fGrabbedInfo)
    {
    // Magus erstellen
    var cb = CreateObject(MAGE, 0, 10, GetOwner());
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

private func AdjustPortrait()
{
  // Mein Portrait stammt auch nicht von mir?
  if(GetPortrait(this, true) != GetID())
  {
    // Neues Portrait setzen
    var iRandoms = Random(GetPortraitCount()) + 1;
    SetPortrait(Format("%d", iRandoms), this, GetID());
  }
}

private func GetPortraitCount() { return 2; }

/* Itemlimit */
public func MaxContentsCount() { return 3; }

/* Spezialtaste: Inventarwechsel */ 
protected func ControlSpecial() 
{ 
  [$CtrlInventoryDesc$|Image=KMSI]  
  // Inventory verschieben 
  return (ShiftContents(0,0,0,1)); 
}

/* Kontextmenü (Einträge vom Zauberclonk löschen */
public func ContextComboHelp(object pByObject)
{
  [""|Image=CXIN|Condition=Never]
  return 1;
}
  
public func ContextCombo(pByObject)
{
  [""|Image=MCMC|Condition=Never]
}
  
public func ContextQuickspell(object pByObject, int iSelection)
{
  [""|Image=MCMQ|Condition=Never]
  return 1;
}

public func ContextMagic(object pByObject)
{
  [""|Image=MCMS|Condition=Never]
  return 1;
}

private func Never() { return; }

// Zaubern
public func ControlDigDouble()
{
  var pObj = Contents();
  if(ReadyToMagic()) if(!Contained())
  if(pObj) if(MysticTime(pObj))
    if(!GetEffect("MysticDelay", pObj))
      // Zauberaktion starten: Dies zaubert direkt im Schwimmen; ansonsten setzt es die Magieaktivität, die verzögert zaubert
      return (SetMagicAction(CastAny(pObj)));
  return _inherited();
}

public func ExecMagic(pObj)
{
  pObj = CastAny(pObj);
  if(!pObj) return 0;
  if(GetEffect("MysticDelay", pObj)) return 0;
  pMagicObject = pObj;
  if(ActivateMystic(pObj))
    if(!pAimer)
      AddEffect("MysticDelay", pMagicObject, 1, MysticTime(pMagicObject));
  return 1;
}

private func SetMagicAction(idForSpell)
{ 
  // Ein Zielzauber? Dann keine Aktion setzen
  if(idForSpell) if(IsAimerSpell(CastAny(idForSpell))) return ExecMagic(idForSpell);
  // Sonst Standartausführung
  return _inherited(CastC4ID(idForSpell));
}

public func SpellFailed(id idSpell, object pByCaller)
{
  // Umleiten an eigentliche Zauberquelle? (Buch, Zauberturm, etc.)
  var pSpellOrigin = pAimedSpellOrigin;
  pAimedSpellOrigin = 0;
  if (pSpellOrigin && pSpellOrigin != this)
    // Auch bei nicht erfolgreicher Umleitung abbrechen: Das zaubernde Objekt hat im Normalfall die Zutaten/Zauberenergie für den
    // Zauber bereit gestellt, und diese sollten nicht an den Clonk zurück gegeben werden
    return (pSpellOrigin->~SpellFailed(idSpell, this));
  // Ist es ein eigener Objektzauber?
  // -> Zauberdelay entfernen
  if(pMagicObject)
    RemoveEffect("MysticDelay", pMagicObject);
}

public func SpellSucceeded(id idSpell, object pByCaller)
{
  // Delay setzten
  AddEffect("MysticDelay", pMagicObject, 1, MysticTime(pMagicObject));
  // Objekt vom Zauber löschen
  pMagicObject = 0;
  // Dannach Standartausführung
  return _inherited(idSpell, pByCaller);
}

public func ActivateMystic(pObj)
{
  var id = GetID(pObj);
  // Zauber im Mystiker gespeichert?
  if(eval(Format("this->~ActivateMystic%i(Object(%d))", id, ObjectNumber(this)))) return 1;
  // oder vll doch im Objekt selber?
  return pObj->~ActivateMystic(this);
}

public func MysticTime(pObj)
{
  var id = GetID(pObj);
  var iTime;
  // Zeit im Mystiker gespeichert?
  if(iTime = eval(Format("this->~MysticTime%i(Object(%d))", id, ObjectNumber(this)))) return iTime;
  // oder vll doch im Objekt selber?
  return pObj->~MysticTime();
}

public func IsAimerSpell(pObj)
{
  var id = GetID(pObj);
  // Zeit im Mystiker gespeichert?
  if(eval(Format("this->~IsAimerSpell%i()", id))) return 1;
  // oder vll doch im Objekt selber?
  return pObj->~MysticTime();
}

// Aktivierung nach Zielen je nach Objekt verarbeiten
public func ActivateAngle(object pCaller, int iAngle)
{
  if(eval(Format("this->~ActivateAngle%i(Object(%d), %d)", GetID(pMagicObject), ObjectNumber(pCaller), iAngle))) return 1;
  pMagicObject->~ActivateAngle(pCaller, iAngle);
  return 1;
}

public func AimingAngle(int iAngle)
  {
  // Zauber weg?
  if (!pAimedSpell) return OnAimerAbort(iAngle);
  // Zielaktion setzen, wenn im Laufen. Das ist etwas ungeschickt, weil damit
  // die Magic-Aktion abgebrochen wird, die der Magier bereits bis zur Haelfte
  // durchgefuehrt hat. Dummerweise laesst sich frueher nicht feststellen, ob
  // der auszufuehrende Zauber einen Aimer brauchen wird...
  if(GetActMapVal("Name", "AimMagic") )
    if(GetAction() == "Walk" || GetAction() == "Magic")
      SetAction("AimMagic");

  // Phase anpassen
  if(GetAction() == "AimMagic")
    {
    // Auch richtigen Winkel verwenden wenn nach links gedreht
    var iHalfAngle = iAngle;
    if(iHalfAngle < 0) iHalfAngle = -iHalfAngle;
    SetPhase(BoundBy((iHalfAngle + 9) / 18, 0, 9) );
    }
  // Weitergabe an den Zauber
  if(eval(Format("this->~AimingAngle%i(Object(%d), %d)", GetID(pMagicObject), ObjectNumber(this), iAngle))) return 1;
  return pMagicObject->~AimingAngle(this, iAngle);
}

public func AbortAiming(pCaller)
{
  // Weitergabe an den Zauber
  eval(Format("this->~AbortAiming%i(Object(%d))", GetID(pMagicObject), ObjectNumber(pCaller)));
  return 1;
}

public func Collection2(pObj)
{
  // Effekt zum Anzeigen der Zauberladung
  if(MysticTime(pObj))
    AddEffect("Mystic", pObj, 1, 5, this);
}

public func Ejection(pObj)
{
  // Wird nach dem Ablegen nicht mehr gebraucht
  RemoveEffect("Mystic", pObj);
}

func FxMysticStart(pTarget, iNumber, fTmp)
{
  if(fTmp) return;
  var iPlace = 0;
  // Ein freies Bit suchen
  for(var i=0; i< ContentsCount()+2; i++)
  {
    if(!(iObjectsMask & (2**i)))
    {
      iPlace = i;
      iObjectsMask |= (2**i);
      EffectVar(0, pTarget, iNumber) = i;
      break;
    }
  }
  // Je nach Platz ein Grafikteil setztn, damit die Objekte
  // im Inventar nicht zusammengefasst werden
  SetGraphics(0, pTarget, GetID(pTarget), 23, GFXOV_MODE_Picture);
  SetClrModulation(RGBa(i,i,i,255), pTarget, 23);
  // Grafik setzen
  SetGraphics(0, pTarget, GetID(pTarget), 20, GFXOV_MODE_Picture, 0, 2);
  SetGraphics(0, pTarget, GetID(pTarget), 21, GFXOV_MODE_Picture, 0, 2);
  SetGraphics(0, pTarget, GetID(pTarget), 22, GFXOV_MODE_Picture);
  // Farben und Größe anpassen
  EffectCall(pTarget, iNumber, "Timer");
}

func FxMysticTimer(pTarget, iNumber, iTime)
{
  // Nicht geladen?
  if(GetEffect("MysticDelay", pTarget))
  {
    // Dann keinen Effekt anzeigen
//    SetClrModulation(RGBa(0,0,0,255), pTarget, 20);
//    SetClrModulation(RGBa(0,0,0,255), pTarget, 21);
    // Aufladeeffekt
    SetObjDrawTransform(1400-Mod(iTime,100)*4, 0, 0, 0, 1400-Mod(iTime,100)*4, 0, pTarget, 20);
    SetObjDrawTransform(1400-Mod(iTime+50,100)*4, 0, 0, 0, 1400-Mod(iTime+50,100)*4, 0, pTarget, 21);
    var iCol = 200*GetEffect("MysticDelay", pTarget, 0, 6)/MysticTime(pTarget);
    SetClrModulation(RGBa(200-iCol+50,iCol+50,50,255-255*Mod(iTime,100)/100), pTarget, 20);
    SetClrModulation(RGBa(200-iCol+50,iCol+50,50,255-255*Mod(iTime+50,100)/100), pTarget, 21);
    return;
  }
  // Größe und Transparenz anpassen
  SetObjDrawTransform(1000+Mod(iTime,100)*5, 0, 0, 0, 1000+Mod(iTime,100)*5, 0, pTarget, 20);
  SetObjDrawTransform(1000+Mod(iTime+50,100)*5, 0, 0, 0, 1000+Mod(iTime+50,100)*5, 0, pTarget, 21);
  SetClrModulation(RGBa(80,80,200,255*Mod(iTime,100)/100), pTarget, 20);
  SetClrModulation(RGBa(80,80,200,255*Mod(iTime+50,100)/100), pTarget, 21);
}

func FxMysticStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return;
  if(!pTarget) return;
  iObjectsMask ^= (2**EffectVar(0, pTarget, iNumber));
  // Grafikeffekt entfernen
  SetGraphics(0, pTarget, 0, 20, 0);
  SetGraphics(0, pTarget, 0, 21, 0);
  SetGraphics(0, pTarget, 0, 22, 0);
  SetGraphics(0, pTarget, 0, 23, 0);
}

public func IsMystic() { return 1; }

/* * * * * * * * * * * */
/* Objekte mit Zaubern */
/* * * * * * * * * * * */

/* Asche */
private func ActivateMysticASHS(pClonk)
{
  return CreateObject(MHTB,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeASHS()
{
  return 36*10;
}

/* Kohle */
private func ActivateMysticCOAL(pClonk)
{
  Sound("Magic*");
  Sound("Rip", 0, pObj);
  CastParticles ("NoGravSpark", 10, 20, 0, 0, 100, 150, RGB(180,150,150), RGB(180,150,150));
  // Fliegende Objekte umkehren
  for(var pObj in FindObjects(Find_OCF(OCF_HitSpeed1), Find_NoContainer(), Find_Category(C4D_Object), Find_Distance(150)))
  {
    SetSpeed(-GetXDir(pObj),-GetYDir(pObj), pObj);
    CreateParticle("NoGravSpark", AbsX(GetX(pObj)), AbsY(GetY(pObj)), 0, 0, 150, RGB(180,150,150));
  }
  return 1;
}

private func MysticTimeCOAL()
{
  return 36*3;
}

/* Beton */
private func ActivateMysticCNCR(pClonk)
{
  return CreateObject(MBRG,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeCNCR()
{
  return 36*30;
}

/* Kristall */
private func ActivateMysticCRYS(pClonk)
{
  return CreateObject(MLGT,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeCRYS()
{
  return 36*10;
}

private func IsAimerSpellCRYS() { return 1; }

/* Erde */
private func ActivateMysticERTH(pClonk)
{
  return CreateObject(MGRP,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeERTH()
{
  return 36*30;
}

/* Gold */
private func ActivateMysticGOLD(pClonk)
{
  return AddEffect("InvisPSpell", pClonk, 200, 175, 0, MINV);
}

private func MysticTimeGOLD()
{
  return 36*120;
}

/* Eis */
private func ActivateMysticICE1(pClonk)
{
  return CreateObject(EXTG,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeICE1()
{
  return 36*5;
}

/* Flüssiggranit */
private func ActivateMysticLIQG(pClonk)
{
  return CreateObject(MFFW,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeLIQG()
{
  return 36*40;
}

/* Lehm */
private func ActivateMysticLOAM(pClonk)
{
  return;
}

private func MysticTimeLOAM()
{
  return 0;
}

/* Metall */
private func ActivateMysticMETL(pClonk)
{
  return 0;
}

private func MysticTimeMETL()
{
  return 0;
}

/* Erz */
private func ActivateMysticORE1(pClonk)
{
  return CreateObject(MBLS,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeORE1()
{
  return 36*25;
}

private func IsAimerSpellORE1() { return 1; }

/* Stein */
private func ActivateMysticROCK(pClonk)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("BigRockNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
  return 1;
}

private func MysticTimeROCK()
{
  return 36*10;
}

private func IsAimerSpellROCK() { return 1; }

private func ActivateAngleROCK(pCaller, iAimAngle)
{
  // Zauber
  Sound("Magic*");
  var id = BIRK;
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir, pRock;
  var iAngle;
  for(var i=0; i<4; i++)
  {
    pRock = CreateContents(id, pCaller);
    iAngle = iAimAngle+RandomX(-10,10);
    if(id!=BIRK) AddEffect("IntArrowVanish",pRock,1,3,0,MGBW);

    // Austrittsgeschwindigkeit ermitteln 
    iOutXDir = Sin(iAngle, 100);
    iOutYDir = -Cos(iAngle, 100);
    // Austrittsposition... 
    iOutX = Sin(iAngle, 25 * GetCon() / 100);
    iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
    // Drehung 
    iOutR = iAngle; 
    if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
    // Besitzer des Projektils setzen
    pRock->SetOwner(pCaller->GetOwner());
    // Farbe!
    pRock->SetClrModulation(RGB(100,100,80));
    // Stein abfeuern
    Exit(pRock, iOutX,iOutY,iOutR, 0,0,iOutRDir );
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    if(id->~IsArrow()) pRock->~Launch();
  }
  return 1;
}

/* Sand */
private func ActivateMysticSAND(pClonk)
{
  return CreateObject(MGHL,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeSAND()
{
  return 36*13;
}

/* Schneeball */
private func ActivateMysticSNWB(pClonk)
{
  return CreateObject(MGSW,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeSNWB()
{
  return 36*8;
}

private func IsAimerSpellSNWB() { return 1; }

/* Schwefel */
private func ActivateMysticSPHR(pClonk)
{
  return CreateObject(MBOT,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeSPHR()
{
  return 36*30;
}

/* Holz */
private func ActivateMysticWOOD(pClonk)
{
  return CreateObject(MGBW,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeWOOD()
{
  return 36*3;
}

private func IsAimerSpellWOOD() { return 1; }

/* Holigon */
private func ActivateMysticHLGN(pClonk)
{
  return CreateObject(ELX1,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeHLGN()
{
  return 36*30;
}

/* gebrochenes Holigon */
private func ActivateMysticBHLG(pClonk)
{
  return CreateObject(ELX2,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeBHLG()
{
  return 36*25;
}

/* Zapnest */
private func ActivateMysticZAPN(pClonk)
{
  AddEffect("PossessionSpell");
  return CreateObject(GZ9Z,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeZAPN()
{
  return 36*30;
}

/* Monsterei */
private func ActivateMysticMEGG(pClonk)
{
  // Monster erzeugen
  var pMons = CreateObject(MONS, 0, 10, GetOwner(pClonk));
  SetDir(GetDir(pClonk), pMons);
  SetCon(60, pMons);
  DoEnergy(10, pClonk);
  // Der Clonk wird zum Monster
  return AddEffect("PossessionSpell", pMons, 200, 10, 0, GetID(), pClonk, 35 * 60 * 2);
}

private func MysticTimeMEGG()
{
  return 36*30;
}

/* Feuermonsterei */
private func ActivateMysticFMEG(pClonk)
{
  // Monster erzeugen
  var pMons = CreateObject(FMNS, 0, 10, GetOwner(pClonk));
  SetDir(GetDir(pClonk), pMons);
  SetCon(60, pMons);
  DoEnergy(10, pClonk);
  // Der Clonk wird zum Monster
  return AddEffect("PossessionSpell", pMons, 200, 10, 0, GetID(), pClonk, 35 * 60 * 2);
}

private func MysticTimeFMEG()
{
  return 36*30;
}

/* Tierübernahme */

protected func FxPossessionSpellStart(pTarget, iEffectNumber, fTemp, pCaster, iTime)
{
 // Keine temporären Aufrufe
 if (fTemp) return;

 // Den Zauberer, sein Portrait sowie die Zeit speichern
 EffectVar(0, pTarget, iEffectNumber) = pCaster;
 EffectVar(1, pTarget, iEffectNumber) = iTime;
 EffectVar(3, pTarget, iEffectNumber) = GetPortrait(pCaster);
 EffectVar(4, pTarget, iEffectNumber) = GetPortrait(pCaster, 1);

 // Die Aktion des Zauberers auf Walk zurücksetzen, und ihn in das Tier hineinversetzen.
 pCaster->SetAction("Walk");
 Enter(pTarget, pCaster);
 
 // Den Controller anpassen
 SetController(GetController(pCaster),pTarget);
 
 // Das Portrait anpassen
 SetPortrait("random", pCaster, GetID(pTarget));

 // Effekte
 CastParticles("PxSpark", 5, 35, AbsX(GetX(pTarget)), AbsY(GetY(pTarget)), 5*8, 5*50, RGB(0,255,0), RGB(0,100,0));
 Sound("Magic1");
}

protected func FxPossessionSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
 // Wachsen lassen
 if(GetCon(pTarget)<100) DoCon(4, pTarget);
 // Nach einer festgelegten Zeit abbrechen
 if (iEffectTime >= EffectVar(1, pTarget, iEffectNumber)) return -1;

 // Countdown für die letzten 10 Sekunden
 if (iEffectTime >= (EffectVar(1, pTarget, iEffectNumber)-10*35))
 {
  var pSorcerer = EffectVar(0, pTarget, iEffectNumber);
  PlayerMessage(GetOwner(pSorcerer), "@%d", pTarget, EffectVar(1, pTarget, iEffectNumber) / 35 - iEffectTime / 35);
 }
 // Clonk tot? Effekt beenden
 if(!GetAlive(EffectVar(0, pTarget, iEffectNumber)) || !EffectVar(0, pTarget, iEffectNumber)) RemoveEffect("PossessionSpell", pTarget);
}

protected func FxPossessionSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
 // Keine temporären Aufrufe
 if (fTemp) return;

 var pSorcerer = EffectVar(0, pTarget, iEffectNumber);

 // Das Tier verliert die Countdown Message
 PlayerMessage(GetOwner(pSorcerer), "", pTarget);

 // Das Portrait des Zauberers anpassen
 SetPortrait(EffectVar(3, pTarget, iEffectNumber), pSorcerer, EffectVar(4, pTarget, iEffectNumber));

 // Effekte
 CastParticles("PxSpark", 5, 35, AbsX(GetX(pTarget)), AbsY(GetY(pTarget)), 5*8, 5*50, RGB(0,255,0), RGB(0,100,0));
 Sound("DeMagic");

 // Das Tier entfernen und den Zauberer entlassen
 RemoveObject(pTarget, 1);
}

protected func FxPossessionSpellDamage (object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  // Schaden durch Säure ignorieren
  if(iCause == 38) return;
  // Schaden an den Clonk weiterleiten
  DoEnergy(iDmgEngy/1000, EffectVar(0, pTarget, iEffectNumber));
  // Clonk tot? Effekt beenden
  if(!GetAlive(EffectVar(0, pTarget, iEffectNumber))) RemoveEffect("PossessionSpell", pTarget);
  // Monster selbst bekommt keinen Schaden
  return;
}

/* Brandbombe */
private func ActivateMysticFBMP(pClonk)
{
  return CreateObject(AUHE,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeFBMP()
{
  return 36*8;
}

/* Schießpulver */
private func ActivateMysticGUNP(pClonk)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("MeteorNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
  return 1;
}

private func MysticTimeGUNP()
{
  return 36*20;
}

private func IsAimerSpellGUNP() { return 1; }

private func ActivateAngleGUNP(pCaller, iAimAngle)
{
  // Zauber
  Sound("Magic*");
  var id = METO;
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir, pRock;
  var iAngle;
  for(var i=0; i<2; i++)
  {
    pRock = CreateContents(id, pCaller);
    iAngle = iAimAngle+RandomX(-10,10);
    if(id!=BIRK) AddEffect("IntArrowVanish",pRock,1,3,0,MGBW);

    // Austrittsgeschwindigkeit ermitteln 
    iOutXDir = Sin(iAngle, 100);
    iOutYDir = -Cos(iAngle, 100);
    // Austrittsposition... 
    iOutX = Sin(iAngle, 25 * GetCon() / 100);
    iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
    // Drehung 
    iOutR = iAngle; 
    if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
    // Besitzer des Projektils setzen
    pRock->SetOwner(pCaller->GetOwner());
    // Kleinere Sprengung
    pRock->SetExplosionBase(1);
    // Stein abfeuern
    Exit(pRock, iOutX,iOutY,iOutR, 0,0,iOutRDir );
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    if(id->~IsArrow()) pRock->~Launch();
  }
  return 1;
}

/* T-Flint */
func ActivateMysticTFLN(pCaster) {
  var pClonk, pComboObj, pBlast;
  pClonk=pCaster;

  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this, pCaster)) return 1;

  var iNum = AddEffect("AirblastNSpell",0,130,1,0,GetID(),90*(GetDir(pCaster)*2-1),GetX(pCaster),GetY(pCaster),pCaster);
  if (iNum <= 0) { return RemoveObject(); }
  if (pComboObj = FindContents(SPHR, pCaster))
  {
    pBlast = pComboObj;
  }
  EffectVar(4, 0, iNum) = pBlast;
  EffectVar(5, 0, iNum) = GetOwner();
  EffectVar(7, 0, iNum) = 5+Random(5);
  Sound("MgWind*");
  RemoveObject();
  return 1;
}

private func MysticTimeTFLN()
{
  return 36*20;
}

private func IsAimerSpellTFLN() { return 1; }

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngleTFLN(object pCaller, int iAngle)
  {
  var pBlast, pComboObj;
  if (pComboObj = FindContents(SPHR, pCaller))
  {
    pBlast = pComboObj;
  }
  // its magic!
  var iNum = AddEffect("AirblastNSpell",0,130,1,0,GetID(),iAngle,GetX(pCaller),GetY(pCaller),pCaller);
  if (iNum <= 0) { return RemoveObject(); }
  Sound("MgWind*");
  
  EffectVar(4, 0, iNum) = pBlast;
  EffectVar(5, 0, iNum) = GetOwner();
  EffectVar(7, 0, iNum) = 5+Random(5);

  return 1;
  }

// EffectVars: 0 - Winkel
//             1 - Entfernung vom Start (0...MaxRange)
//             2 - Start:X
//             3 - Start:Y

func FxAirblastNSpellStart(object pTarget, int iNumber, int iTemp, int iAngle, int iX, int iY, object pCaller) {
  if(iTemp) return;

  // Richtung speichern
//  var iAngle = EffectVar(0,pTarget,iNumber);// = (iAngle+90)*(GetDir(pCaller)*2-1);
  EffectVar(0,pTarget,iNumber) = iAngle;
  EffectVar(1,pTarget,iNumber) = 20;

  // Position
  EffectVar(2,pTarget,iNumber) = iX;
  EffectVar(3,pTarget,iNumber) = iY;
  
  // Controller merken
  EffectVar(6,pTarget,iNumber) = GetController(pCaller);

  // Zeit merken
  if(!EffectVar(7,pTarget,iNumber)) EffectVar(7,pTarget,iNumber) = 10;
  
  // globalen Wind beeinflussen
  SetWind(GetWind()+Sin(iAngle,40));
}

func FxAirblastNSpellTimer(object pTarget, int iNumber, int iTime) {
  var iPos, pObj, iX, iY, iSpeed, iRandom, iAngle, iSize, iStep;

  var iPos = EffectVar(1,pTarget,iNumber);
  // löschen falls schon zu weit alles weggeblasen
  if(iPos >= EffectCall(pTarget,iNumber,"MaxRange")*EffectVar(7,pTarget,iNumber)/10) return -1;

  iStep = -iPos/18+EffectCall(pTarget,iNumber,"MaxRange")/17;

  iX = EffectVar(2,pTarget,iNumber);
  iY = EffectVar(3,pTarget,iNumber);

  iAngle = EffectVar(0,pTarget,iNumber);
  iSize = iStep+iPos/5;
  
  iX += Sin(iAngle, iPos);
  iY -= Cos(iAngle, iPos);

  //Log("%d,%d",iSize,iStep);


  //  Log("%d,%d,%d,%d",iX+Sin(iAngle,iPos)-iSize,iY-Cos(iAngle,iPos)-iSize,iSize*2,iSize*2);

  // Objekte wegblasen
  while(pObj=FindObject(0,iX-iSize,iY-iSize,iSize*2,iSize*2,OCF_Collectible|OCF_Alive,0,0,NoContainer(),pObj))
    {
    if(Stuck(pObj)) continue;
    if (GetProcedure(pObj) == "ATTACH") continue; // Keine getragenen Schilde, Alchemiebeutel, etc.
    iSpeed=iStep*6;
    if(iSpeed<0) iSpeed=0;
   
    // Sound fürs Wegblasen
    Sound("MgWndB", false, pObj);

    //Log("%s: %d; %d,%d",GetName(pObj),iSpeed,Sin(iAngle,iSpeed+20),-iSpeed);

    if(GetAlive(pObj))
      {
      iRandom=Random(3);
      //DoEnergy(-1-iRandom,pObj);
      //pObj->~CatchBlow(-1-iRandom,this);
      Fling(pObj,Sin(iAngle,iSpeed+20)/9,-iSpeed/2/9);
      }
    else
      {
       SetSpeed(Sin(iAngle,iSpeed+20),-iSpeed/2,pObj);
       
       // Richtigen Controller setzen
       SetController(EffectVar(6,pTarget,iNumber),pObj);
      }
    }
    
  // Explosionen bei Kombo
  var iMat;
  if (EffectVar(4, pTarget, iNumber))
    if (GBackSolid(iX, iY))
      {
      iMat = GetMaterial(iX,iY);
      if (iMat >= 0 && iMat != Material("Vehicle"))
        {
        Explode(17, CreateObject(ROCK, iX,iY,EffectVar(5, pTarget, iNumber)));
        return -1;
        }
      }

  var clr = iPos*255/EffectCall(pTarget,iNumber,"MaxRange");
  CreateParticle("PSpark",iX,iY,0,0,iSize*15,RGBa(150,160,255,clr*127/255));

  // alle 20 Pixel wird nach Objekten gesucht
  EffectVar(1,pTarget,iNumber)+=iStep;
}

func FxAirblastNSpellMaxRange() { return 170; }

/* Super-T-Flint */
private func ActivateMysticSTFN(pClonk)
{
  if(!FindContents(FLNT)) CreateContents(ARRW, pClonk);
  return CreateObject(MWND,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeSTFN()
{
  return 36*8;
}

/* Flint */
private func ActivateMysticFLNT(pClonk)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("MeteorNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
  return 1;
}

private func MysticTimeFLNT()
{
  return 36*20;
}

private func IsAimerSpellFLNT() { return 1; }

private func ActivateAngleFLNT(pCaller, iAimAngle)
{
  // Zauber
  Sound("Magic*");
  var id = DFLM;
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir, pRock;
  var iAngle;
  for(var i=0; i<2; i++)
  {
    pRock = CreateContents(id, pCaller);
    iAngle = iAimAngle+RandomX(-10,10);
    if(id!=BIRK) AddEffect("IntArrowVanish",pRock,1,3,0,MGBW);

    // Austrittsgeschwindigkeit ermitteln 
    iOutXDir = Sin(iAngle, 100);
    iOutYDir = -Cos(iAngle, 100);
    // Austrittsposition... 
    iOutX = Sin(iAngle, 25 * GetCon() / 100);
    iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
    // Drehung 
    iOutR = iAngle; 
    if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
    // Besitzer des Projektils setzen
    pRock->SetOwner(pCaller->GetOwner());
    // Stein abfeuern
    Exit(pRock, iOutX,iOutY,iOutR, 0,0,iOutRDir );
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    if(id->~IsArrow()) pRock->~Launch();
  }
  return 1;
}

/* Super Flint */
private func ActivateMysticSFLN(pClonk)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("MeteorNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
  return 1;
}

private func MysticTimeSFLN()
{
  return 36*20;
}

private func IsAimerSpellSFLN() { return 1; }

private func ActivateAngleSFLN(pCaller, iAimAngle)
{
  // Zauber
  Sound("Magic*");
  var id = FBMP;
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir, pRock;
  var iAngle;
  for(var i=0; i<1; i++)
  {
    pRock = CreateContents(id, pCaller);
    iAngle = iAimAngle+RandomX(-10,10);
    if(id!=BIRK) AddEffect("IntArrowVanish",pRock,1,3,0,MGBW);

    // Austrittsgeschwindigkeit ermitteln 
    iOutXDir = Sin(iAngle, 100);
    iOutYDir = -Cos(iAngle, 100);
    // Austrittsposition... 
    iOutX = Sin(iAngle, 25 * GetCon() / 100);
    iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
    // Drehung 
    iOutR = iAngle; 
    if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
    // Besitzer des Projektils setzen
    pRock->SetOwner(pCaller->GetOwner());
    // Stein abfeuern
    Exit(pRock, iOutX,iOutY,iOutR, 0,0,iOutRDir );
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    if(id->~IsArrow()) pRock->~Launch();
  }
  return 1;
}

/* Teraflint */
private func ActivateMysticEFLN(pClonk)
{
  return CreateObject(MDBT,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeEFLN()
{
  return 36*40;
}

/* Knochen */
private func ActivateMysticBONE(pClonk)
{
  Sound("Wave");
  ScheduleCall(pClonk, "BONEEffect", 3, 4);
  ScheduleCall(pClonk, "BONESlay", 1, 4*3);
  return 1;
}

private func MysticTimeBONE()
{
  return 36*40;
}

protected func BONEEffect() {
  var iRand=RandomX(4*3*70,4*3*90);
  CreateParticle("PxSpark",0,0,0,0,iRand,RGB(40,255,0));
}

protected func BONESlay() {
  var pClonk;
  var iRand=RandomX(4*3*70,4*3*90);
  while(pClonk=FindObject(0,-iRand/6,-iRand/6,iRand/3,iRand/3,OCF_CrewMember,0,0,0, pClonk))
    if(this != pClonk)
      if(!GetEffect("BulletPoision",pClonk)) AddEffect("BulletPoision",pClonk,20,3,0,PBLT,GetOwner());
}

/* Schädel */
private func ActivateMysticSKUL(pClonk)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("BonesNSpell", 0, 125)) return iResult!=-1 && RemoveObject();
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this)) return 1;
  return 1;
}

private func MysticTimeSKUL()
{
  return 36*20;
}

private func IsAimerSpellSKUL() { return 1; }

private func ActivateAngleSKUL(pCaller, iAimAngle)
{
  // Zauber
  Sound("Magic*");
  var id = BONE;
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir, pRock;
  var iAngle;
  for(var i=0; i<5; i++)
  {
    pRock = CreateContents(id, pCaller);
    iAngle = iAimAngle+RandomX(-10,10);
    if(id!=BIRK) AddEffect("BoneTumble",pRock,1,3,pCaller);

    // Austrittsgeschwindigkeit ermitteln 
    iOutXDir = Sin(iAngle, 100);
    iOutYDir = -Cos(iAngle, 100);
    // Austrittsposition... 
    iOutX = Sin(iAngle, 25 * GetCon() / 100);
    iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
    // Drehung 
    iOutR = iAngle; 
    iOutRDir = 0;
  
    // Besitzer des Projektils setzen
    pRock->SetOwner(pCaller->GetOwner());
    // Stein abfeuern
    Exit(pRock, iOutX,iOutY,iOutR, 0,0,iOutRDir );
    SetR(Random(360), pRock);
    SetRDir(100, pRock);
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    if(id->~IsArrow()) pRock->~Launch();
  }
  return 1;
}

public func FxBoneTumbleTimer(pTarget,iNumber)
{
  SetXDir(GetXDir(pTarget)+RandomX(-10,10), pTarget);
  SetYDir(GetYDir(pTarget)+RandomX(-10,10), pTarget);
  EffectVar(0,pTarget,iNumber)++;
  SetClrModulation(RGBa(255,255,255, EffectVar(0,pTarget,iNumber)*5), pTarget);
  if(EffectVar(0,pTarget,iNumber)>=200/5) RemoveObject(pTarget);
  else if(Contained(pTarget)) RemoveObject(pTarget);
}

/* Muschel */
private func ActivateMysticSHEL(pClonk)
{
  return CreateObject(MDFL,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeSHEL()
{
  return 36*40;
}

/* Torpedo */
private func ActivateMysticTRP1(pClonk)
{
  return CreateObject(FRFS,0,0,GetOwner(pClonk))->Activate(pClonk);
}

private func MysticTimeTRP1()
{
  return 36*20;
}
