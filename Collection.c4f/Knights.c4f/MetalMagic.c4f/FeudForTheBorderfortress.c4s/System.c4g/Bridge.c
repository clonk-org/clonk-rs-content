/*-- Appendto Bridge --*/

#strict
#appendto BRDG

public func Damage(foo){
  if(GetDamage() > 43)Incinerate(this());
  return(1);
}
