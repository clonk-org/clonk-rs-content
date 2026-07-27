/*--- Zweihandschwert ---*/

#strict 2

/* Aufschlag */
protected func Hit()
{
  Sound("SwordHit*");
  return 1;
}

public func CarryLimit()
{
 return 1;
}

public func Entrance(pObj)
{
  pObj->~GotSword(this);
}

public func Departure(pObj)
{
  pObj->~LostSword(this);
}

public func ControlThrow(pCaller)
{
  return pCaller->~UseTwoHandSword(this);
}

/* Aufwertungszauberkombo: Mit Zweihänder wird der Clonk zum Paladin */
public func GetRevaluationCombo(object pClonk) { return PLDN; }

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }
