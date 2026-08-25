/*-- Altar --*/

#strict 2
#appendto ALTR

public func UpgradeTo() { return CHPL; }

func Initialize() {
  AddEffect("CheckEnemy",this,20,5,this);
  AddEffect("DestructionCheck", this, 1, 0, this);
  Message("@100%", this);
  return _inherited();
}

func FxDestructionCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  EffectVar(0, pTarget, iEffectNumber) += iDmg;
  Message("@%d%", this, 100-(EffectVar(0, pTarget, iEffectNumber)*100/GetMaxEnergy()));
  if(EffectVar(0, pTarget, iEffectNumber)>GetMaxEnergy()) { Incinerate(); Message("", this); return -1; }
}

public func GetMaxEnergy() { return 200; }

public func GetRepairValue()
{
  return EffectVar(0,this,GetEffect("DestructionCheck", this))*GetValue()/GetMaxEnergy();
}

protected func FxCheckEnemyTimer(pTarget,iEffectNumber,iEffectTime)
{
  var aEnemies = FindObjects(Find_OCF(OCF_CrewMember),Find_Owner(-1),Find_Distance(40));
  for(var pClonk in aEnemies)
  {
    pClonk->CreateParticle("NoGravSpark", 0, 0, 0, -20, 80, RGB(255));
    DoEnergy(-1, pClonk);
  }
  return 1;
}
