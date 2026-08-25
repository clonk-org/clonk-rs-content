/*-- iLive --*/

#strict 2
#appendto GIDL

public func NeedLevel() { return 2; }

public func CheckEmenyIntervall() { return 5; }

public func GetMaxEnergy() { return 200; }

protected func FxCheckEnemyTimer(pTarget,iEffectNumber,iEffectTime)
{
  var aEnemies = FindObjects(Find_OCF(OCF_CrewMember),Find_Owner(-1),Find_Distance(40));
  for(var pClonk in aEnemies)
  {
    var iEff;
    if(iEff = GetEffect("StrongSlow", pClonk)) EffectVar(0, pClonk, iEff) = 5;
    else AddEffect("StrongSlow", pClonk, 10, 5, this);
  }
  return 1;
}

func FxStrongSlowStart(pTarget, iNumber, fTmp)
{
  SetPhysical("Walk", GetPhysical("Walk",0,0, GetID(pTarget))/2, 2, pTarget);
  if(!fTmp)
    EffectVar(0, pTarget, iNumber) = 5;
}

func FxStrongSlowTimer(pTarget, iNumber)
{
  pTarget->CastParticles("FSpark", 8, 10, 0, 0, 40, 50, RGB(200,200,100), RGB(200,200,100), pTarget);
  if(!EffectVar(0, pTarget, iNumber)--) return -1;
}

func FxStrongSlowStop(pTarget, iNumber, iReason, fTmp)
{
  ResetPhysical(pTarget, "Walk");
}
