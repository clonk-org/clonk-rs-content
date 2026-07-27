/*-- Wings --*/

#strict 2

protected func Check()
{
  var pVat = GetActionTarget();   
  SetAction(GetAction(pVat));
  SetPhase(GetPhase(pVat));
  SetDir(GetDir(pVat));
  
  SetOwner(GetOwner(pVat));
  SetVisibility(GetVisibility(pVat));
  
}

protected func AttachTargetLost()
{
 return RemoveObject();
}
