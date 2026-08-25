/*-- Meuchler --*/

#appendto ASAS
#strict 2

public func Never() { return false; }
public func ContextHide()
{
  [Condition=Never]
}