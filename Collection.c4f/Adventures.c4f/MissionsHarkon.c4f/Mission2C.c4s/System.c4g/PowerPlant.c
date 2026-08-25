/* Kraftwerk */

#strict 2

#appendto POWR
#appendto FNDR

private func Burning()
{
	if(GetID() == FNDR)
	{
		// Rauch
		Smoke(+23,Random(3)-24,Random(5)+8);
		return;
	}
  // Rauch
  Smoke(-15,-20,12);
  Smoke(-1,-23,8);
  // Energieerzeugung
  DoEnergy(+25);
  // Aber nicht zurücksetzen, brennt hier ewig!
}

// Tür bleibt zu!
protected func ActivateEntrance(pObj) { }

// Braucht hier immer Energy
func CheckEnergyNeedChain() { return 1; }