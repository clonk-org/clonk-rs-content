/*-- Speer --*/

#strict 2

/* Status */
public func IsSpear() 
{ 
  return 1; 
}

public func CarryLimit()
{
  // Nur eines tragbar
  return 1;
}

public func Entrance()
{
  // Damit es die richtige Austrittsrotation annimmt
  SetRDir();
}

/* Werfen */
public func ControlThrow()
{
  // Träger ermitteln
  var pClonk = Contained();
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return 0;
  // Träger bereit zum Werfen?
  if (GetAction(pClonk) != "Walk" && GetAction(pClonk) != "Jump" && GetAction(pClonk) != "Ride" && GetAction(pClonk) != "RideStill") return;
  // Beim Träger Wurfaktion setzen (reitend)
  if (Contained()->~IsRiding())
    return ObjectSetAction(Contained(), "RideThrowSpear");
  // Beim Träger Wurfaktion setzen (normal)
  if (!ObjectSetAction(Contained(), "ThrowSpear")) return 0;
  // Fertig
  return 1;
}

/* Einsetzen */

public func Activate(pClonk)
{
  [$TxtSpearAttack$|Condition=CanUse]
  if(pClonk->~CanUse(GetID(this)))
  if(pClonk->~StartSpearAttack(this)) return true;
  return false;
}

// An Clonk weiterleiten
public func CanUse(pClonk) { return pClonk->~CanUseSpearAttack(); }

/* Ist eine Handwaffe */
public func IsFightWeapon(pClonk)
{
  //if(pClonk->~CanUse(GetID(this)))return 1;
  if(GetID(pClonk) == KAND)return 1;
  return;
}

/* Kampfaktion */
public func GetFightAction()
{
  return "SpearFight";
}

/* Schlagaktion */
public func GetStrikeAction()
{
    return "SpearThrust";
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return 3;
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return 10;
}

/* Schlag */
public func StrikeSound()
{
  Sound("SwordHit*");
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }

