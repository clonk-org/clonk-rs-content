/* Objekte, denen die KI ausweichen sollte */

#strict 2

#appendto CLNK

static const g_iLeader = 1930;
static g_alreadyPatrol;

func ActGetDialog(idID, iIndex, pTalker1, pTalker2)
{
	if(idID == NOTY)
	{
		if(!g_alreadyPatrol)
		{
			if(iIndex == 0)
				return ActDialog("$Corpse01$", pTalker1, pTalker2);
			if(iIndex == 1)
				return ActDialog("$Corpse02$", pTalker2, pTalker1);
			if(iIndex == 2)
				return ActDialog("$Corpse03$", pTalker2, pTalker1);
			pTalker2->ScheduleCall(pTalker2, "SetActivity", 1, 1, PTRL);
			g_alreadyPatrol = 1;
		}
		else
		{
			if(iIndex == 0)
				return ActDialog("$Corpse04$", pTalker1, pTalker2);
			if(iIndex == 1)
				return ActDialog("$Corpse05$", pTalker2, pTalker1);
			if(iIndex == 2)
				return ActDialog("$Corpse06$", pTalker2, pTalker1);
		}
/*		Object(2799)->SetActivity([FOLW, pTalker2]);
		Object(2785)->SetActivity([FOLW, pTalker2]);
		Object(2778)->SetActivity([FOLW, pTalker2]);
		Object(2792)->SetActivity([FOLW, pTalker2]);*/
		return 0;
	}
	return _inherited(idID, iIndex, pTalker1, pTalker2, ...);
}

func TestActivityPossible(pItem, aExtra)
{
	var idID = NONE;
	if(GetType(pItem) == C4V_C4Object) idID = GetID(pItem);
	if(GetType(pItem) == C4V_C4ID) idID = pItem;
	if(idID == NOTY)
	{
		if(GetAlive(Object(g_iLeader))) return 1;
	}
	return _inherited(pItem, aExtra, ...);
}

func ExecActivityItem(pItem, aExtra) // 1 for keep 0 for finish
{
	var idID = NONE;
	if(GetType(pItem) == C4V_C4Object) idID = GetID(pItem);
	if(GetType(pItem) == C4V_C4ID) idID = pItem;
	if(idID == NOTY)
	{
		if(g_alreadyPatrol) return 1;
		var pLeader = Object(g_iLeader);
		FinishActivity();
		SetActivity([TALK, pLeader, NOTY]);
		return 0;
	}
	if(idID == PTRL)
	{
		var aList = [
		[847, 232],
    [869, 319],
    [707, 409],
    [498, 552],
    [668, 630],
    [939, 659],
    [874, 831],
    [1014, 540],
    [792, 541],
		];
		var iIndex = (ExtraGetValue(aExtra, 1, 0)+1) % GetLength(aList);
		var aGuards = [Object(9723), Object(9724), Object(9725), Object(9726)];
		SetActivity([PTRL, iIndex]);
		SetActivity(LOOK);
		SetActivity([MOVE, aList[iIndex]]);
		var i = 1;
		var iDir = 1;
		for(var pGuard in aGuards)
		{
			pGuard->SetActivity([WAIT, this], 1);
			var iX = aList[iIndex][0]+iDir*(i+1)/2*10;
			var iY = aList[iIndex][1];
			var myDir = (iDir == 1);
			pGuard->SetActivity([LOOK,[iX, iY, myDir]]);
			pGuard->SetActivity([MOVE, [iX, iY, myDir]]);
//			AddEffect("IntWait", pGuard, 1, 4*i);
			i++;
			iDir = -iDir;
		}
		return 0;
	}
	return _inherited(pItem, aExtra, ...);
}