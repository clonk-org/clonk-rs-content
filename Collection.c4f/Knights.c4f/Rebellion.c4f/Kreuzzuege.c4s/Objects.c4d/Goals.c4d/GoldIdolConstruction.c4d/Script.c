/*-- Goldstatuenbau --*/

#strict
#include GOAL

public func IsFulfilled()
{
  var pObj;
  while(pObj=FindObject(GIDL, 2600, 0, 600, 500, OCF_Fullcon,,,, pObj))
    if(!pObj->LocalN("film_object"))
      return(1);
}

public func Activate(plr) 
{
  if(IsFulfilled(plr)) 
    MessageWindow("$done$", plr);
  else
    MessageWindow("$notdone$", plr);
}
