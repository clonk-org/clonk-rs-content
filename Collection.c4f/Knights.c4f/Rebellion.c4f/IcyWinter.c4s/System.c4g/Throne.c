/*-- Thron --*/

#strict

#appendto THRN

private func UpdateState()
  {
  var obj = IsOccupied();
  if (obj) {
    if (obj->GetID() == MAGE && !Random(5)) {
      obj->DoMagicEnergy(2);
      obj->DoEnergy(1);
    }
    if (Not(SEqual(GetAction(),"Occupied")))
      return(SetAction("Occupied"));
  }
  else
    if (Not(SEqual(GetAction(),"Empty")))
      return(SetAction("Empty"));
  return(0);
  }

private func IsOccupied() { return(_inherited()); }

