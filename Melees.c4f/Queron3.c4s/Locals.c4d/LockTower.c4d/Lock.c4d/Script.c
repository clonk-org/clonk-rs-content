/* Schloss */

#strict 2

local iRefID, pActTarget, pKey;
// Local0: RefID
// Local1: Kontrolliertes Objekt

protected func Initialize()
	{
	SetAction("None");
	SetRefID(Random(15));
	}

/* Statussetzung */

public func SetTarget(object pTarget)
	{
	pActTarget = pTarget;
	return 1;
	}
public func GetTarget(object pTarget) { return pActTarget; }

public func SetRefID(int iID)
	{
	iRefID = iID;
  // Steckenden Schlüssel ggf. mit anpassen
  if (pKey) pKey->SetRefID(iID);
	return 1;
  }

public func GetRefID() { return iRefID; }

public func GetKey() { return pKey; }

/* Steuerung */

protected func ControlThrow(object pCaller)
	{
  [$TxtPutGetKey$]

  if (!pKey)  // Schlüssel reinstecken
		{
		var key = FindMatchingKey(pCaller), anyKey = FindAnyKey(pCaller);
		if (!key)
			{
			if (!anyKey)
				return Message("$MsgNoKey$", this);
			else
				return Message("$MsgNoFit$", this);
			}
		else return CollectKey(key);
		}
	else return EjectKey();
 	}

protected func ControlDig()
	{
  [$TxtTurnKey$]
  return TurnKey();
 	}

protected func ControlUp()
	{
  [$TxtInfo$]
  Message("Ref ID #%d|made in Claiwan",, GetRefID());
  return 1;
	}

public func CollectKey(object pNewKey)
	{
	if (pKey || pNewKey->GetRefID()!=GetRefID()) return;
	pNewKey->Enter(this);
	pKey = pNewKey;
	SetAction("Key");
	return 1;
	}

public func EjectKey()
	{
	if (!pKey) return;
	// Raus mit dem Schlüssel
	Exit(pKey);
	pKey = 0;
	SetAction("None");
	SetDir(0);
	return 1;
	}

public func TurnKey(object pClonk)
	{
	if (!pKey)
		{
		Message("$MsgNoKey$", this);
		return 0;
		}
	Sound("UnlockGate");
	SetDir(1-GetDir());
  // Angeschlossenes Objekt aktivieren
  pActTarget->~Activate();
	}

public func CreateKey()
{
	if (pKey) return;
	var key = CreateObject(_KEY,,, NO_OWNER);
	key->SetRefID(iRefID);
	CollectKey(key);
	return key;
}

/* Transfer */

public func FindMatchingKey(object pContainer)
	{
  // Ist in Par() ein passender Schlüssel im Inventar?
  return FindObject2(Find_Container(pContainer), Find_Func("IsKey"), Find_Func("RefIDMatch", iRefID));
  }
  
private func FindAnyKey(object pContainer)
{
	return FindObject2(Find_Container(pContainer), Find_Func("IsKey"));
}
