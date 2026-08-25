#strict
#appendto CLNK

public func ContextPermanentModeTurnOn(pCaller) 
{
  [$TxtPermanentModeTurnOn$|Image=ELEC|Condition=GrabbingNonPermanentElevator]
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ELEC|Condition=GrabbingPermanentElevator]
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      GetActionTarget()->DoControlAuto(this());
}

private func GrabbingNonPermanentElevator()
{
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsNotInPermanentMode())
        return(1);
  return(0);        
}

private func GrabbingPermanentElevator()
{
  if (GetAction() eq "Push")
    if (GetActionTarget()->~IsElevator())
      if (GetActionTarget()->IsInPermanentMode())
        return(1);
  return(0);        
}
