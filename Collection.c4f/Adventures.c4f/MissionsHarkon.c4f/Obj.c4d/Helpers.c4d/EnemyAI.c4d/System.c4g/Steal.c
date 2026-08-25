/* Objekte, denen die KI ausweichen sollte */

#strict 2

#appendto CLNK

local aStealItems;

func AddStealItem(pObj)
{
	if(aStealItems == 0)
		aStealItems = [];
	aStealItems[GetLength(aStealItems)] = pObj;
	pObj->Enter(this);
	SetWarning(WNST);
}

func CanBeStolen() { return !!aStealItems; }

func DoSteal(pClonk)
{
	for(var pObj in aStealItems)
		pObj->Enter(pClonk);
	aStealItems = 0;
	RemWarning(WNST);
}