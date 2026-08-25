/*--- Granit ---*/

#strict

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

protected func Hit3()
{
  Sound("RockHit*");
  ShakeFree(GetX(), GetY(), 5+Random(10));
  CastObjects(_GSR, 5+Random(5), 60-Random(Random(Random(50))), 0, -3);
  return(RemoveObject());
}
