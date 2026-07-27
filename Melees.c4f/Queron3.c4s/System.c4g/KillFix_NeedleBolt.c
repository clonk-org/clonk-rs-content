/* Nadelbolzen: Killverfolgung */

#strict 2

#appendto NBLT

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
  AddEffect("NeedleBolt", pObject, 120, 30, 0, GetID(), fHeadShot, GetOwner()); // + Owner
  RemoveObject();
}

func FxNeedleBoltStart(pTarget, iNumber, fTmp, fHeadShot, iOwner)
{
  if(fTmp) return;
  if(fHeadShot) EffectVar(0, pTarget, iNumber) = 2;
  else EffectVar(0, pTarget, iNumber) = 1;
  EffectVar(1, pTarget, iNumber) = iOwner;
}

func FxNeedleBoltTimer(pTarget, iNumber, iTime)
{
  DoEnergy(-EffectVar(0, pTarget, iNumber), pTarget, false, 0, EffectVar(1, pTarget, iNumber) + 1); // + iCausedBy
  if(iTime>30*30) if(!Random(6)) return -1;
}
