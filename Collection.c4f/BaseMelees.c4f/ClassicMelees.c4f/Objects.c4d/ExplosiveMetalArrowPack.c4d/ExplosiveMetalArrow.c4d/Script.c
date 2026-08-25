/*-- MetalSprengpfeil --*/

#strict

#include ARRW

local Armed;

/* Abfeuern */

public func Launch(object pByObject)
{
  // Der Sprengpfeil wird nur durch korrektes Abfeuern scharf gemacht
  Armed = 1;    
  return(_inherited() );
}

/* Pfeileigenschaften */
public func PackTo() {  return(_EMP); }
public func ArrowStrength() { return(4+Random(4)); }
public func ArrowTumbleRandom() {return(4);}
public func ArrowStopsOnHit() { return(0); }

/* Aufschlag */

protected func Hit()
{
  if (Armed)
    Explode(Random(10) + 20);
}

/* Treffer */

private func HitTarget(pTarget)
{
 if(pTarget->~IsArrowTarget(this())) return(Hit());
 
 return(_inherited(pTarget));
}
