/*-- Brot --*/

#strict 2

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return(1);
}

public func Eat(object pClonk)
{
  pClonk->~Feed(80/6);
	if(GetAction() != "Exist") SetAction("Exist");
	if(GetPhase() == 5)
		RemoveObject();
	else
		SetPhase(GetPhase()+1);
  return 1;
}

func EnterTable() { SetObjDrawTransform(500, 0, 0, 0, 500, 2000); }
func LeaveTable() { SetObjDrawTransform(1000, 0, 0, 0, 1000); }

/* Produkteigenschaften */

public func IsOvenProduct() { return 1; }
public func IsFood() { return 1; }