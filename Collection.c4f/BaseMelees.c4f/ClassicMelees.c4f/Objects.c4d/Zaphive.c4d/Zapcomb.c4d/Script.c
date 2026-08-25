/*-- Honig --*/

#strict

protected func Hit()
{
Sound("WoodHit*");
  //Honig
   CastObjects(_HON,8,4);
   CastObjects(_HON,1,4);
   CastObjects(_HON,6,4);
   CastObjects(_HON,1,10);
   CastObjects(_HON,12,8);
   CastObjects(_HON,13,9);
RemoveObject();
}

public func Activate(pByObject)
{
  [$CtrlEatDesc$]
  return(Eat(pByObject));
}

public func Eat(pByObject)
{
  pByObject->Feed(40);
  RemoveObject();
  return(1);
}
