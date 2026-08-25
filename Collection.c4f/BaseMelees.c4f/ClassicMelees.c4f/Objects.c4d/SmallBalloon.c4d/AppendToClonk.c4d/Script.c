/*-- Ballonsicht --*/

#strict

#appendto CLNK

protected func ControlDownDouble()
{
  // Steuerung an Winde/Ballon weiterleiten
  if(GetSmallBaloon())
      if(GetAction() eq "Walk")
	{
	SetPlrView(GetOwner(), GetSmallBaloon());
	return(1);
	}
  return(_inherited(...));
}

public func ContextWatchOut(pCaller)
{
  [$CtxWatchOut$|Image=_BLN|Condition=GetSmallBaloon]
  ControlDownDouble();
  return(1);
}

protected func GetSmallBaloon()
{
if(GetID(Contents())==CL5P)
	{
	var pBaloon = LocalN("pConnected",Contents());
    	if(GetID(pBaloon)==_BLN)
		return(pBaloon);
	}
return(0);
}