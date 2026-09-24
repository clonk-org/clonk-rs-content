/*-- Leitungsbau --*/

#strict
#include GOAL

public func IsFulfilled()
{
  // Mine versorgt?
  var pObj=GameCall("UBRLTarget");
  if(pObj)
    if(!EnergyCheck(100, pObj))
      return(0);
  // fertig?
  return(1);
}

protected func Activate(iPlr)
{
  if (IsFulfilled()) return(MessageWindow("$MsgMinePowered$",iPlr));
  return(MessageWindow("$MsgMineNeedsPower$",iPlr));
}
