/*--- Granit ---*/

#strict
#include _CBL

protected func Hit()
{
  Sound("RockHit*");
  if(!fActivated) return();
  ShakeFree(GetX(), GetY(), 5+Random(10));
  CastObjects(_GSR, 5+Random(5), 60-Random(Random(Random(50))), 0, -3);
  return(RemoveObject());
}