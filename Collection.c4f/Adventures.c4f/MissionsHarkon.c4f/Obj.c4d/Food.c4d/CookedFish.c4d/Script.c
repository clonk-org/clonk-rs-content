/*-- Gekochter Fisch --*/

#strict 2

protected func Construction() 
{
  var pEnv;
  if (pEnv=FindObject(CLFS))
    pEnv->CLFS::Colorize(this());
  else
    SetColorDw(RGB(255,255,255));
}

public func Activate(pByObject)
{
  [$CtrlEatDesc$]
  return Eat(pByObject);
}

public func Eat(object pClonk)
{
	if(GetPhase() == 5) return;
  pClonk->~Feed(80/5);
	if(GetAction() != "Exist") SetAction("Exist");
	SetPhase(GetPhase()+1);
  return 1;
}

func EnterTable() { SetObjDrawTransform(600,0,0,0,600,2500); }
func LeaveTable() { SetObjDrawTransform(1000, 0, 0, 0, 1000); }

/* Produkteigenschaften */

public func IsFood() { return GetPhase() != 5; }

/* Aufwertungszauberkombo: Mit Fisch wird der Clonk zum Aquaclonk */
public func GetRevaluationCombo(object pClonk) { return ACLK; }
