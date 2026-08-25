/*--- Kristallkugel ---*/

#strict
#include _CBL

public func ExplodeSize() { return(1); }

protected func Hit()
{   
  Sound("Crystal*");
  if(!fActivated) return();
  CastObjects(_BLX, RandomX(20,30), 100, 0, 0); 
  Explode(ExplodeSize());
  return(1);
}