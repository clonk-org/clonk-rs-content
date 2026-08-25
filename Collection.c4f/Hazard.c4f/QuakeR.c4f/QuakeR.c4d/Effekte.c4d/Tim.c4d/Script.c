/*-- TIM --*/

#strict
#appendto TIM2

public func Spawn()
{
  if(!Contents()) return(RemoveObject());

  AddSpawnEffect(Contents(), Contents()->GetColorDw());
  Sound("respawn");
  RemoveObject(0,1);
  return(1);
}
