/*-- Krummschert --*/

#strict 2

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return 1;
}

/* Kampfaktion */
public func GetFightAction()
{
  return "ScimitarFight";
}

/* Schlagaktion */
public func GetStrikeAction()
{
  if (Random(2))
    return "ScimitarThrust";
  else
    return "ScimitarStrike";
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return 2;
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return 8;
}

/* Schlag */
public func StrikeSound()
{
  Sound("SwordHit*");
}

/* Aufschlag */
protected func Hit()
{
  Sound("SwordHit*");
  return 1;
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 0; }

// Kann in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return 1; }

/* Aufwertungszauberkombo: Mit Krummschert wird der Clonk zum Kanderier */
public func GetRevaluationCombo(object pClonk) { return KAND; }
