/*-- Gebäude nicht betretbar --*/

#strict

#appendto DOOR

protected func ActivateEntrance(object pObj)
{
  if(GetID() == TWR2) return(_inherited(pObj));
  SetCommand(pObj, "Jump");
  SetEntrance(0);
  return(1);
}
