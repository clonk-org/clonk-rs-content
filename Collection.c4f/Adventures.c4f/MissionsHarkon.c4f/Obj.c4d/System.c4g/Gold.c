/*-- Gold --*/

#appendto GOLD
#strict 2

protected func RejectEntrance(object pContainer)
{
  if(GetPlayerType(GetOwner(pContainer))==C4PT_User)
  {
    Message(Format("{{GOLD}} %d", GetValue()), pContainer);
    DoWealth(GetOwner(pContainer), GetValue());
    Sound("Cash");
    return RemoveObject();
  }
}
