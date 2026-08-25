/*-- Leitungsbau --*/

#strict
#include GOAL

public func IsFulfilled()
{
  // Mine versorgt?
  if(var pObj=GameCall("UBRLTarget"))
    if(!EnergyCheck(100, pObj))
      return(0);
  // fertig?
  return(1);
}

protected func Activate(iPlr)
{
  if (IsFulfilled()) return(MessageWindow("Die Mine ist versorgt",iPlr));
  return(MessageWindow("Die Mine braucht noch Strom",iPlr));
}
