/*-- Remove items on deah --*/

#strict 2
#appendto CLNK

public func Initialize()
{
	AddEffect("RemoveItemsOnDeath", this, 1, 0, this);
	return _inherited(...);
}

global func FxRemoveItemsOnDeathStop(pTarget, iNumber, iReason, fTmp)
{
	if(fTmp) return;
  while(Contents(0, pTarget)) Contents(0, pTarget)->RemoveObject();
}