/* Fahrstuhlkorb */

#strict 2

#appendto ELEC

private func MoveToWaitingClonk() {
  // Nur wenn Strom da ist
  var pObj = FindObject(POWR);
  if(pObj && pObj->ActIdle()) return 0;
  return _inherited(...);
}

private func SetMoveTo() {
  // Nur wenn Strom da ist
  var pObj = FindObject(POWR);
  if(pObj && pObj->ActIdle()) return 0;
  return _inherited(...);
}