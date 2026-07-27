/* Der Clonk: Objektaufnahmeprozess wird von Effekt geleitet (extra für Fischclonk) */

#strict

#appendto CLNK

local fCollect;

public func RejectCollect(id idObject, object pObject, bool fEffectHandling)
{
	// Geldsäcke immer aufnehmbar
	if (idObject == MBAG) return 0;
	// Sonst Effekt erstellen, der den Fischskin-Effekt evtl. temporär entfernt
	if (fCollect > -1) 
		{
		AddEffect("Inventory", this, 100, 0, this,, idObject, pObject); // Fischclonk hat prio 101!
		return fCollect; // wurde gesetzt
		}
	
	return _inherited(idObject, pObject, ...);
}

private func FxInventoryStart(object pTarget, int iNr, int iTemp, id idObj, object pObj)
{
	if (iTemp) return;
	fCollect = -1;
	fCollect = RejectCollect(idObj, pObj); // Ok, der Fischskin Effekt sollte gerade weg sein, das nutzen wir aus!
	return -1;
}
