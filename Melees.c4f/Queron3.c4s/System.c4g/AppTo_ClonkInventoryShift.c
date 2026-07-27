/* Clonk: Inventarwechsel */

#strict 2

#appendto CLNK

// Clonks können fairheitshalber 2 Sachen aufnehmen

public func MaxContentsCount()
{
	if (GetID() == CLNK) return 2;
	if (GetID() == ACLK) return 2;
	//if (GetID() == MAGE) return 2;
	else return _inherited(...);
}

// Inventarwechsel auch für Clonks

protected func ControlSpecial()
{
  //Nur für Clonk und Aqua, der Rest hat schon bzw behandelt selbst (Kombo unso)
  if (GetID() == CLNK || GetID() == ACLK)
    return ShiftContents(0, 0, 0, true);
  return _inherited(...);
}
