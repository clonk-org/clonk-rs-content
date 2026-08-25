/*-- Gebäude --*/

#strict 2

#appendto WTWR

#appendto IDOL

#appendto TWR2
#appendto CPT2

#appendto WRKS
#appendto MWKS

#appendto HUT2
#appendto HUT3
#appendto AHUT

#appendto ALTR
#appendto CHPL

#appendto ACLD

func Initialize() {
  if(OnFire()) return _inherited();
  if(!GetEffect("CheckEnemy", this) && CheckEmenyIntervall())
    AddEffect("CheckEnemy",this,20,CheckEmenyIntervall(),this);
  if(!GetEffect("DestructionCheck", this))
    AddEffect("DestructionCheck", this, 1, 0, this);
  Message("@100%", this);
  return _inherited();
}

func FxDestructionCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  EffectVar(0, pTarget, iEffectNumber) += iDmg;
  Message("@%d%", this, Max(100-(EffectVar(0, pTarget, iEffectNumber)*100/GetMaxEnergy()), 1));
  if(EffectVar(0, pTarget, iEffectNumber)>GetMaxEnergy()) { Incinerate(); Message("", this); return -1; }
}

public func GetRepairValue()
{
  return EffectVar(0,this, GetEffect("DestructionCheck", this))*GetValue()/GetMaxEnergy();
}

public func DoUpgrade()
{
  RemoveEffect("CheckEnemy", this);
  var iLive = 100-(EffectVar(0, this, GetEffect("DestructionCheck", this))*100/GetMaxEnergy());
  RemoveEffect("DestructionCheck", this);
  ChangeDef(UpgradeTo());
  Call("Upgrade2", iLive);
}

public func Upgrade2(iLive)
{
  Initialize();
  EffectVar(0, this, GetEffect("DestructionCheck", this)) = GetMaxEnergy()*(100-iLive)/100;
  Message("@%d%", this, iLive);
  return Call("Upgraded");
}

public func Upgraded() { return _inherited(); }

// Settings

public func GetUpgradeValue() { return _inherited(); }
public func UpgradeTo() { return _inherited(); }
public func NeedLevel() { return _inherited(); }

public func CheckEmenyIntervall() { return _inherited(); }
public func GetMaxEnergy() { return _inherited(); }
