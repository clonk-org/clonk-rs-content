/*-- Miesmuschel --*/

#strict

public func IsShell(){ return(1); }
/* - Muschelumwandler - */
public func ShellChangeAble(){ return(1); }
public func ShellChangeTime(){ return(50); }
public func ShellChangeMats(){ return(BCOR); }
public func ShellChangeAmount(){ return(1); }

protected func Hit()
{
  Sound("MetalHit*");
  return(1);
}

public func Activate(pObj) {
  [$TxtEat$]
  Eat(pObj);
  return(1);
}

public func Eat(pObj) {
  pObj->~Feed(20);
  RemoveObject();
}
