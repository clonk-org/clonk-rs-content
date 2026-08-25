/*-- Keks --*/

#strict 2

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return 1;
}

public func Eat(object pClonk)
{
  pClonk->~Feed(10);
	if(GetAction() != "Exist") SetAction("Exist");
	if(GetPhase() == 2)
		RemoveObject();
	else
		SetPhase(GetPhase()+1);
  return 1;
}

/* Produkteigenschaften */

public func IsOvenProduct() { return 1; }
public func IsFood() { return 1; }