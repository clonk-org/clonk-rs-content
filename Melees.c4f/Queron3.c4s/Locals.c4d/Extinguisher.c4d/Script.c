/*-- Löscher --*/

// script mod v1.0 by miniwipf
#strict 2

protected func Check()
{
	for (var clonk in FindObjects(Find_OCF(OCF_OnFire), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive)))
		AddEffect("Extinguisher", clonk, 250, DelayTime(), this);
}

public func FxExtinguisherEffect(string szNewName, object pTarget, int iNr, int iNewNr)
{
	if (szNewName == GetEffect(, pTarget, iNr, 1))
		return -1;
}

public func FxExtinguisherTimer(object pTarget, int iNr)
{
	// Jetzt löschen
	Extinguish(pTarget);
	return -1;
}

protected func Activate(int iPlr)
{
  MessageWindow(GetDesc(), iPlr);
  return 1;
}

public func DelayTime() { return 35*RandomX(4, 5); }
