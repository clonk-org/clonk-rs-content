/*-- Aufzug --*/

#appendto ELEC
#strict

private func DoControlDown(pObj)
{  
  if(GetPlayerType(GetOwner(pObj))==C4PT_User)
    AI_AlarmCurious(this, 300);
  return _inherited(pObj, ...);
}

private func DoControlUp(pObj)
{
  if(GetPlayerType(GetOwner(pObj))==C4PT_User)
    AI_AlarmCurious(this, 300);
  return _inherited(pObj, ...);
}
