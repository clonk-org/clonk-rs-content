/*-- Kristallpfeil --*/

#strict

#include ARRW

local Armed;

/* Abfeuern */

public func Launch(object pByObject)
{
  // Der Kristallpfeil wird nur durch korrektes Abfeuern scharf gemacht
  Armed = 1;    
  return(_inherited() );
}

/* Pfeileigenschaften */
public func PackTo() {  return(CARP); }
public func ArrowStrength() { return(1+Random(2)); }
public func ArrowTumbleRandom() {return(2);}

/* Treffer */

private func HitTarget(pTarget)
{
 if (Armed)
 var iChange = - GetMagicEnergy (pTarget) * (4+Random(5))/16;
 if (iChange == 0) return(_inherited(pTarget));
 DoEnergy (iChange, pTarget);
 DoMagicEnergy(iChange, pTarget);
 Sound("Electric");
 CastParticles("MSpark", 5, 50, 0,0, 100, 200, RGBa(100,100,255,128), RGBa(0,0,255,0));
 CreateParticle("MSpark", 0,0, 1000, RGBa(50,50,255,150));
 RemoveObject(this());
 return(_inherited(pTarget));
}
