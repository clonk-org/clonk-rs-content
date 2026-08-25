/*-- Ritter --*/

#strict

#appendto KNIG

protected func FindAttachedItems(object after)
{
  var obj = after;
  while(obj = FindObject(0, 0, 0, 0, 0, 0, 0, this(), 0, obj) )
    if((GetID(obj) != _CAP) && // Captainindikator
       (GetID(obj) != _BOB))   // Burner
      if(GetProcedure(obj) S= "ATTACH")
        return(obj);
  return(0);
}

