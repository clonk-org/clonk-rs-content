/*-- Bolzen --*/

#strict 2

#include BOLT

/* Paketeigenschaften */
public func PackTo() { return NBTP; }
public func IsBolt() { return 1; }
public func IsArrow() { return 0; }

public func BoltStrength() { return 20+Random(6); }

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
    PlayerMessage(GetOwner(), ".", pObject);
    fHeadShot = 1;
  }
  if(!GetAlive(pObject))
    return RemoveObject();
  AddEffect("NeedleBolt", pObject, 120, 30, 0, GetID(), fHeadShot);
  RemoveObject();
}

func FxNeedleBoltStart(pTarget, iNumber, fTmp, fHeadShot)
{
  if(fTmp) return;
  if(fHeadShot) EffectVar(0, pTarget, iNumber) = 2;
  else EffectVar(0, pTarget, iNumber) = 1;
}

func FxNeedleBoltTimer(pTarget, iNumber, iTime)
{
  DoEnergy(-EffectVar(0, pTarget, iNumber), pTarget);
  if(iTime>30*30) if(!Random(6)) return -1;
}

func FxNeedleBoltDamage(pTarget, iNumber, iDmgEngy, iCause)
{
  // Bei Heilung wird der Bolzen entfernt
  if(iDmgEngy>0) RemoveEffect(0, pTarget, iNumber);
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
