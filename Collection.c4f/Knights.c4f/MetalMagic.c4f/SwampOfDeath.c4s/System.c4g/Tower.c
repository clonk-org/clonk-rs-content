/*-- Turm --*/

#strict 2
#appendto TWR2
#appendto CPT2

func Initialize() {
  AddEffect("DestructionCheck", this, 1, 0, this);
  Message("@100%", this);
  return _inherited();
}

func FxDestructionCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  EffectVar(0, pTarget, iEffectNumber) += iDmg;
  Message("@%d%", this, 100-(EffectVar(0, pTarget, iEffectNumber)*100/GetMaxEnergy()));
  if(EffectVar(0, pTarget, iEffectNumber)>GetMaxEnergy())  { Incinerate(); Message("", this); return -1; }
}

public func GetMaxEnergy() { return 300+300*(GetID()==CPT2); }

public func GetRepairValue()
{
  return EffectVar(0,this, GetEffect("DestructionCheck", this))*GetValue()/GetMaxEnergy();
}

public func GetUpgradeValue() { if(GetID()==TWR2) return 10; }
public func UpgradeTo() { if(GetID()==TWR2) return CPT2; }

public func DoUpgrade()
{
  RemoveEffect("CheckEnemy", this);
  var iLive = 100-(EffectVar(0, this, GetEffect("DestructionCheck", this)))*100/GetMaxEnergy();
  RemoveEffect("DestructionCheck", this);
  ChangeDef(CPT2);
  Call("Upgraded", iLive);
}

public func Upgraded(iLive)
{
  SetAction("Ready");
  Initialize();
  EffectVar(0, this, GetEffect("DestructionCheck", this)) = GetMaxEnergy()*(100-iLive)/100;
}


func FxDestructionCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  EffectVar(0, pTarget, iEffectNumber) += iDmg;
  Message("@%d%", this, 100-(EffectVar(0, pTarget, iEffectNumber)*100/300));
  if(EffectVar(0, pTarget, iEffectNumber)>300)  { Incinerate(); Message("", this); return -1; };
}
