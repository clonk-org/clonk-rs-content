/*-- Dach --*/

#strict

#include CPW2 // Burgteilfunktion

protected func Initialize()
{
  // Basisimplementation
  _inherited();
  // Fertig
  return 1;
}

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (80); }
public func CastlePartHeight() { return (43); }
public func CastlePartTop() { return (-3); }
public func CastlePartBasement() { return(BSC3); }


/* Forschung */

public func GetResearchBase() { return(CPW2); }