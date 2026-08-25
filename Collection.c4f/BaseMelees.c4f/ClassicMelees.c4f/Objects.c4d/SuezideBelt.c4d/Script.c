/*-- Sprenggürtel --*/

#strict

protected func Hit(){Sound("WOOD*");}

public func Activate(pClonk)
{
  [$CtrlActivateDesc$]
  if (pClonk->IsSuezideClonk()) return(1);
  if (!FindNearIdol())
	{
	PlayerMessage(GetOwner(),"$TxtNoIdol$",Contained());
	return(1);
	}
  if (GetID(Contained()) != CLNK)
	{
	PlayerMessage(GetOwner(),"$TxtNoClonk$",Contained());
	return(1);
	}
  pClonk->BuckleBelt();
  Sound("Click");
  RemoveObject();
  return(1);
}

public func FindNearIdol()
{
if (FindObject2(Find_Or(Find_ID(IDOL), Find_ID(GIDL)), Find_Not(Find_Hostile(GetOwner(Contained()))), Find_Distance(50))) return(1);
return(0);
}

/* Ist ein Produkt der Chemiefabrik */
protected func IsChemicalProduct() { return(1); }