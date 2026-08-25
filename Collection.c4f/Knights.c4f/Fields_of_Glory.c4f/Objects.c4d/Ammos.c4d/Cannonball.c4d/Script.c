/*--- Kanonelkugel ---*/

#strict

local fActivated;

public func IsCanonball() { return(1); }

public func ExplodeSize() { return(25); }

public func IsFragile() { return(TRUE); } // Aktiviert bei schnellem Aufprall?

protected func Hit()
{   
  Sound("RockHit*");
  if(!fActivated) return();
  CastObjects(_BLP, RandomX(20,30), 70, 0, 0); 
  Explode(ExplodeSize());
  return(1);
}

protected func Hit3()
{
  if (IsFragile()) fActivated = 1;
  Sound("RockHit*");
  return(1);
}

protected func Departure(pObj)
{
  if(GetID(pObj)==_HLA) fActivated = 1;
  if(GetID(pObj)==CCAN) fActivated = 1;
}

protected func Entrance()
{
  fActivated = 0; 
}

public func IsAnvilProduct() { return(1); }
