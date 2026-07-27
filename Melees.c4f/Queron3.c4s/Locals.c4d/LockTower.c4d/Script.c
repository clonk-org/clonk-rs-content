/*-- Abschließbares Tor --*/

#strict 2

#include CPW2 // Burgteilfunktion

local pGate, pLockLeft, pLockRight;

protected func Initialize()
	{
	 // Tor erzeugen
	pGate = CreateObject(CPSG, 2, 34, GetOwner());
	// Schlösser erzeugen
	pLockLeft = CreateObject(LOCK, -14, 35, GetOwner());
	pLockRight= CreateObject(LOCK, 12, 35, GetOwner());
	//pLockRight->SetRefID(pLockLeft->GetRefID());
	// Schlösser aufs Tor ausrichten
	pLockLeft->SetTarget(pGate);
	pLockRight->SetTarget(pGate);
  // Transferzone
  UpdateTransferZone();
	return inherited(...);
	}

protected func Destruction()
	{
  // Schlösser und Gitter entfernen
  if (pLockLeft) RemoveObject(pLockLeft);
  if (pLockRight) RemoveObject(pLockRight);
  if (pGate) RemoveObject(pGate);
  return inherited(...);
	}
	
/* Transfer */

func UpdateTransferZone()
	{
  if (GetOCF() & OCF_Fullcon)
    SetTransferZone(-19, 18, 38, 39);
  return 1;
  }

public func ControlTransfer(object pClonk, int iTx, int iTy) // (C4Object* pObj, int iTx, int iTy)
	{
	var lock = pLockRight, key;
  if (((GetX()-iTx) * (GetX()-GetX(pClonk))) > 0)
  	// Clonk auf der richtigen Seite: erledigt
  	return 0;
  if (!pGate)
  	// Tor weg: erledigt
  	return 0;
	if (pGate->IsOpen())
		{
		if (AbsY(GetY(pGate)) > 5)
			{
			if (GetAction(pClonk) == "Push")
				AddCommand(pClonk, "UnGrab", lock);
				return 1; // Warten bis ganz offen
			}
		else
			// Tor offen: erledigt
			return 0;
		}
	else
		{
		if (GetX(pClonk) < GetX())  lock = pLockLeft;
		if (GetAction(pClonk) == "Push")
			{
			if (pClonk->GetActionTarget() == lock)
				{
				if (lock->GetKey())
					{
					// Schlüssel drehen
					lock->ControlDig(pClonk);
					return 1;
					//return AddCommand(pClonk, "Dig");
					}
				else
					{
					// Schlüssel reintun
					var key = lock->FindMatchingKey(pClonk), anyKey = lock->FindAnyKey(pClonk);
					if (!key)
						{
						if (!anyKey)
							{
							return lock->Message("$MsgNoKey$", lock);
							}
						else return lock->Message("$MsgNoFit$", lock);
						}
					lock->ControlThrow(pClonk);
					return 1;
					}
				}
				else
					// Was auch immer loslassen
					return AddCommand(pClonk, "UnGrab", lock);
			}
		else
			// Schloss anfassen
			return AddCommand(pClonk, "Grab", lock);
		}
  return 1;
	}

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (40); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BSC1); }


/* Forschung */

public func GetResearchBase() { return(CPW2); }
