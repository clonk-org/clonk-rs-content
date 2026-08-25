/*-- Goldmuschel --*/

#strict

public func IsShell(){ return(1); }
/* - Muschelumwandler - */
public func ShellChangeAble(){ return(1); }
public func ShellChangeTime(){ return(350); }
public func ShellChangeMats(){ return(YCOR); }
public func ShellChangeAmount(){ return(8); }

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
  pObj->~Feed(150);
  RemoveObject();
}
