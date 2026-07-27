/*-- Fadenkreuz --*/

#strict 2

local szCheck;

protected func Initialize()
{
  SetVisibility(VIS_Owner);
}

public func StartCheck(szCheckString, pClonk)
{
  szCheck = szCheckString;
  SetAction("Crosshair", pClonk);
}

protected func Check()
{
  if(!szCheck) szCheck = "*Rifle*";
  if(!WildcardMatch(GetAction(GetActionTarget()), szCheck))
  {
    RemoveTrajectory(GetActionTarget());
    return RemoveObject();
  }
}

public func AttachTargetLost()
{
  RemoveObject();
}
