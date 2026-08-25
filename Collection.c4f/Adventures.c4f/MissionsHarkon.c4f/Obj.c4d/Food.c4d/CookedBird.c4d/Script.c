/*-- Gekochter Vogel --*/

#strict 2

public func Activate(object pClonk)
{
  [$TxtEat$]
  Eat(pClonk);
  return 1;
}

public func Eat(object pClonk)
{
	if(GetPhase() == 6) return;
  pClonk->~Feed(10);
	if(GetAction() != "Exist") SetAction("Exist");
	SetPhase(GetPhase()+1);
  return 1;
}

func EnterTable() { SetObjDrawTransform(450,0,0,0,450,2500); }
func LeaveTable() { SetObjDrawTransform(1000, 0, 0, 0, 1000); }

/* Produkteigenschaften */

public func IsFood() { return GetPhase() != 6; }
public func IsGarbage() { return GetPhase() == 6; }
