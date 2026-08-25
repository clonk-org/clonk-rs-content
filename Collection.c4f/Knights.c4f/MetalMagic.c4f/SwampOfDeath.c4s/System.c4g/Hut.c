/*-- Hütte --*/

#strict 2

#appendto HUT2
#appendto HUT3
#appendto AHUT

public func NeedLevel() { if(GetID()==HUT3) return(3); if(GetID()==HUT2) return(2); return(1); }

public func CheckEmenyIntervall() { return 1; }

public func GetMaxEnergy() { return 200+300*(GetID()==HUT2)+600*(GetID()==HUT3); }

public func UpgradeTo() { if(GetID()==AHUT) return(HUT2); if(GetID()==HUT2) return(HUT3); }

public func GetDistance()
{
  if(GetID()==HUT2) return 60;
  if(GetID()==HUT3) return 80;
  return 40;
}

public func GetHealRandon()
{
  if(GetID()==HUT2) return 10;
  if(GetID()==HUT3) return 8;
  return 15;
}

protected func FxCheckEnemyTimer(pTarget,iEffectNumber,iEffectTime)
{
  var aClonks = FindObjects(Find_OCF(OCF_CrewMember),Find_Not(Find_Owner(-1)),Find_Distance(GetDistance()), Find_NoContainer());
  for(var pClonk in aClonks)
  {
    if(GetEnergy(pClonk)>=GetPhysical("Energy", 0, pClonk)/1000) continue;
    DrawParticleLine("MaterialSpark", GetVertex(0, 0), GetVertex(0, 1), GetX(pClonk)-GetX(), GetY(pClonk)-GetY(), 5, 30, RGB(255), RGB(100));
    if(!Random(GetHealRandon())) DoEnergy(1, pClonk);
    if(!Mod(iEffectTime, 2))
    {
      CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8)-GetX(),GetY(pClonk)+RandomX(-10, 10)-GetY(),0,-20,RandomX(18,38),RGB(210,20,20));
      CreateParticle("PSpark",GetX(pClonk)-GetX(),GetY(pClonk)-GetY(),0,0,180,RGB(16,8,0),pClonk);
    }
    return;
  }
  return 1;
}
