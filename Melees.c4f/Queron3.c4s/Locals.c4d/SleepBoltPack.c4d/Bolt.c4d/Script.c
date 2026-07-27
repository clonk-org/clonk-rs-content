/*-- Schlafbolzen --*/

#strict 2

#include BOLT

/* Paketeigenschaften */
public func PackTo() { return SBTP; }
public func IsBolt() { return 1; }
public func IsArrow() { return 0; }

public func IsDangerousToAirTravel() { return 1; }

local iOldX, iOldY;

/* Wird abgefeuert */
public func Launch()
{
  iOldX=GetX();
  iOldY=GetY();
  SetAction("Shot");
  SetCategory(4);
  SetClrModulation(RGBa(255,255,255,128));
}

public func RejectEntrance(a, b)
{
  if(GetAction()=="Shot") return 1;
  return _inherited(a,b);
}

/* Aufschlag */
protected func Hit()
{
  if(GetAction()=="Shot")
  {
    var target = FindTarget();
    if(target)
      return HitTarget(target);
    SetCategory(1);
    SetClrModulation(RGBa(255,255,255));
  }
  SetAction("Idle");
}

/* Ziel prüfen */
private func InFlight()
{
  SetR(Angle(0, 0, GetXDir(), GetYDir()));
  var target = FindTarget();
  if(target)
    return HitTarget(target);
}

/* Treffer */

private func HitTarget(pObject)
{
  var fHeadShot;
  var iDuration = 60;
  var iFeathers = 5;
  // Rüstung getroffen? Einfach runterfallen, wir können ja nicht stecken bleiben
  if(Random(100) < pObject->~IsShielded())
  {
    Sound("ArrowRicochet*");
    SetXDir();
    SetYDir();
    return;
  }
  // Im oberen Drittel getroffen?
  if(GetY()<GetY(pObject)+GetDefHeight(GetID(pObject))/3+GetDefOffset(GetID(pObject), 1) && !pObject->~HasArmor())
  {
    // Headshot!!
    Message("z", pObject);
    iDuration = 130;
    iFeathers = 20;
  }
  for(var i = 0; i < iFeathers; i++)
    CreateParticle("Feather", GetX(pObject)-GetX(), GetY(pObject)-GetY(), RandomX(-5,5), RandomX(-5,-1), 50+Random(25));
  AddEffect("SleepNSpell", pObject, 123, 40, 0, GetID(), iDuration);
  RemoveObject();
}

protected func FxSleepNSpellStart(object pTarget, int iEffectNumber, int iTemp, int iTime) {
  // temporäre aufrufe
  if(iTemp) return;
  // Scheintoter Zustand
  ObjectSetAction(pTarget,"Dead");
  // mit schnarchen anfangen
  Sound("Snore",0,pTarget,0,0,+1);
  // Zeit bis zum Aufwachen speichern
  EffectVar(0,pTarget,iEffectNumber)=iTime;
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

protected func FxSleepNSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  if (fTemp)
    return;
  // mit schnarchen aufhören
  Sound("Snore",0,pTarget,0,0,-1);
  // clonk hat den Schlaf nicht überstanden :(
  if(!GetAlive(pTarget)) return 1;
  // aufstehen
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  // stehenbleiben
  SetComDir(COMD_Stop,pTarget);
  // Markereffekt entfernen (für KI benötigt)
  RemoveEffect("IntFoundMarker", pTarget);
  // Fertig
  return 1;
}

protected func FxSleepNSpellTimer(object pTarget, int iEffectNumber, int iTime) {
  var iMaxTime=EffectVar(0,pTarget,iEffectNumber);
  // nach einer bestimmten Zeit aufhören
  if(iMaxTime && iTime>=iMaxTime) return(-1);
  // Zzz-Partikel casten
  CreateParticle("Zzz",GetX(pTarget),GetY(pTarget),2+(iTime%50)*3/25+RandomX(-1,+1),-5,60,
                 RGBa(255,255,255,(iTime-iMaxTime/2)**2 * 180*4/iMaxTime**2));
}

protected func FxSleepNSpellDamage(object pTarget, int iEffectNumber, int iDmgEngy) {
  // Bei Damage aufwachen
  RemoveEffect(0,pTarget,iEffectNumber);
  return iDmgEngy;
}

/* Steckt in Zielobjekt */

private func Sticking()
{
  if (!GetAlive(Local(2))) return RemoveObject();
  SetPosition(GetX(Local(2)) + Local(3), GetY(Local(2)) + Local(4));
  if(!Mod(GetActTime(),30)) DoEnergy(-1-Local(5),Local(2));
  if(GetActTime()>30*30) if(!Random(6)) return RemoveObject();
}

public func IsSilent() { return 1; }
