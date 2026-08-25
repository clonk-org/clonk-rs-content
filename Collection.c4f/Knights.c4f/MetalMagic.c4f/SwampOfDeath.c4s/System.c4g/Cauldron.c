/*-- Alchemie --*/

#strict 2
#appendto ACLD

func Initialize() {
  AddEffect("DestructionCheck", this, 1, 0, this);
  Message("@100%", this);
  return _inherited();
}

public func NeedLevel() { return 1; }

func FxDestructionCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  EffectVar(0, pTarget, iEffectNumber) += iDmg;
  Message("@%d%", this, 100-(EffectVar(0, pTarget, iEffectNumber)*100/GetMaxEnergy()));
  if(EffectVar(0, pTarget, iEffectNumber)>GetMaxEnergy()) { Incinerate(); Message("", this); return -1; }
}

public func GetMaxEnergy() { return 300; }

public func GetRepairValue()
{
  return EffectVar(0,this, GetEffect("DestructionCheck", this))*GetValue()/GetMaxEnergy();
}

func FxDestructionCheckDamage(pTarget,iEffectNumber,iDmg,iCause)
{
  EffectVar(0, pTarget, iEffectNumber) += iDmg;
  Message("@%d%", this, 100-(EffectVar(0, pTarget, iEffectNumber)*100/300));
  if(EffectVar(0, pTarget, iEffectNumber)>300)  { Incinerate(); Message("", this); return -1; }
}

/* Produktion */

public func ProductionMenu(pWorker) {
  // Nur wenn inaktiv
  if (!ActIdle()) return 0;
  // Menu erzeugen
  CreateMenu(ACLD, pWorker, this, C4MN_Extra_Value, "$TxtNoPlans$");
  // Menü mit Bauplänen füllen
  var i = 0, def;
  while (def = GetPlrKnowledge(GetOwner(pWorker), 0, i++, C4D_Object))
    if(def->~IsPotion())
      AddMenuItem("$TxtProduceMenu$","ProductionOrder",def,pWorker,0,pWorker);
  // Fertig
  return 1;
}

public func ProductionOrder(idProduct, pWorker) {

  // Verfügbarkeit überprüfen
  if(GetWealth(GetOwner(pWorker)) < GetValue(0, idProduct))
  {
    PlayerMessage(GetOwner(pWorker),"$NotEnoughMoney$",this);
    return 1;
  }
  DoWealth(GetOwner(pWorker),-GetValue(0, idProduct));
  var pPotion = CreateContents(idProduct);
  Collect(pPotion, pWorker);
  Effects();
  // Los geht's
  return 1;
}
