/*-- Kiste --*/

#appendto CHST
#strict 2

protected func Grabbed(object pClonk, bool fGrab)
{
  if(GetPlayerType(GetOwner(pClonk))==C4PT_User)
    AI_AlarmCurious(this, 300);
  return _inherited(...);
}
