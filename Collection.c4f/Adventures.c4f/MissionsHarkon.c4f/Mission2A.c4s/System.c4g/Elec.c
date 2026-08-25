/* Fahrstuhlkorb */

#strict 2

#appendto ELEC

local fDeactivated;

private func MoveToWaitingClonk() {
  // Nur wenn Strom da ist
  if(fDeactivated) return 0;
  return _inherited(...);
}

private func SetMoveTo() {
  // Nur wenn Strom da ist
  if(fDeactivated) return 0;
  return _inherited(...);
}