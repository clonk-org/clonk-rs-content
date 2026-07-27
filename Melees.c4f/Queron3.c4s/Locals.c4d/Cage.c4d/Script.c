/*-- Käfig --*/

#strict 2

local iRefID;

/* Öffnung */

protected func ControlThrow(object pClonk)
  {
  [$CtrlOpen$|Image=_SKE]
  // Schon offen?
  if (GetAction() == "Open") return;
  // Schlüssel dabei?
	var key = FindMatchingKey(pClonk), anyKey = FindAnyKey(pClonk);
	if (!key)
		{
		if (!anyKey)
			return Message("$MsgNoKey$", this);
		else
			return Message("$MsgNoFit$", this);
		}
	else
		{
		// Aufschliessen
		Sound("Connect", this());
		Sound("ChestOpening", this());
		SetAction("Open");
		SetSolidMask();
		GameCallEx("OnCageOpened", this());
		RemoveObject(key);
		}
	}

/* Statussetzung */

public func SetRefID(int iID)
	{
	iRefID = iID;
	return 1;
  }

public func GetRefID() { return iRefID; }

public func FindMatchingKey(object pClonk)
	{
  // Ist in Par() ein passender Schlüssel im Inventar?
  return FindObject2(Find_Container(pClonk), Find_Func("IsKey"), Find_Func("RefIDMatch", iRefID));
  }

private func FindAnyKey(object pContainer)
{
	return FindObject2(Find_Container(pContainer), Find_Func("IsKey"));
}
