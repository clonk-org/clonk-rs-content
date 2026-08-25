/*-- Metalpfeilpaket --*/

#strict

#include ARWP

/* Aufschlag */

protected func Hit()
{
  Sound("MetalHit");
}

/* Paketeigenschaften */

public func UnpackTo() { return(_MAR); }

public func GetResearchBase() { return(ARWP); }