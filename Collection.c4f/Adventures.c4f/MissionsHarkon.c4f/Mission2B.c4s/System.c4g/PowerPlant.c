/* Kraftwerk */

#strict 2

#appendto POWR

private func Burning()
{
  // Rauch
  Smoke(-15,-20,12);
  Smoke(-1,-23,8);
  // Energieerzeugung
  DoEnergy(+25);
  // Aber nicht zurücksetzen, brennt hier ewig!
}

// Tür bleibt zu!
protected func ActivateEntrance(pObj) { }

public func RejectCollect(id idID) { if(idID != COAL && idID != WOOD) return 1; }

// Braucht hier immer Energy
func CheckEnergyNeedChain() { return 1; }