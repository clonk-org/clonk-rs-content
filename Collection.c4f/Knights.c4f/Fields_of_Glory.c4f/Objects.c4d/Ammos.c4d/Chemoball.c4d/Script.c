/*--- Chemiekugel ---*/

#strict
#include _CBL

local pProduced;

public func ExplodeSize() { return(10); }

protected func Hit()
{   
  Sound("RockHit*");
  if(!fActivated) return();
  CastObjects(_BLC, RandomX(10,20), 35, 0, 0); 
  Explode(ExplodeSize());
  return(1);
}

/* Fertigstellung */
protected func Initialize()
{
  // Wurden wir produziert? Dann gibts das leere Ölfass zurück
  if(pProduced && Contained() == pProduced)
    CreateContents(BARL, pProduced);
}

/* Produktion */
public func OnStartProduction(pChemicalPlant)
{
  pProduced = pChemicalPlant;
}

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

public func IsAnvilProduct() { return(0); }