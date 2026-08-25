/*-- Lebensabläufe --*/

#strict 2

#appendto CLNK

func ActGetDialog(idID, iIndex, pTalker1, pTalker2)
{
	if(idID == NOTY)
	{
		if(!1)
		{
			if(iIndex == 0)
				return ActDialog("Ich habe eine Leiche gesehen! Ein Angreifer hat sich eingeschlichen!.", pTalker1, pTalker2);
			if(iIndex == 1)
				return ActDialog("Ok, durchsucht die Burg, irgndwo muss der Meuchler sein.", pTalker2, pTalker1);
			if(iIndex == 2)
				return ActDialog("Keine Panik, wir werden ihn schon finden!", pTalker2, pTalker1);
			pTalker2->ScheduleCall(pTalker2, "SetActivity", 1, 1, PTRL);
		}
		else
		{
			if(iIndex == 0)
				return ActDialog("Eine Leiche wurde gefunden! Ein Meuchler muss da sein.", pTalker1, pTalker2);
			if(iIndex == 1)
				return ActDialog("Das weis ich schon. Wir sind ja schon auf der Suche nach ihm..", pTalker2, pTalker1);
			if(iIndex == 2)
				return ActDialog("Bleib ruhig und wachsam, wir werden ihn schon finden.", pTalker2, pTalker1);
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
	if(idID == TABL)
	{
		if(ExtraGetValue(aExtra, 1))
			if(ExtraGetValue(aExtra, 2) > pItem->GetFreePlaces()) return 0;
	}
	return _inherited(pItem, aExtra, ...);
}

func GetArrow()
{
	var pAct = GetActivity();
	if(GetValueID(pAct) == ARCH) return CreateObject(ARRW);
	return _inherited(...);
}

func ExecActivityItem(pItem, aExtra) // 1 for keep 0 for finish
{
	var idID = NONE;
	if(GetType(pItem) == C4V_C4Object) idID = GetID(pItem);
	if(GetType(pItem) == C4V_C4ID) idID = pItem;
	if(idID == ARCH)
	{
		pItem->ExecActivity(this, ExtraGetValue(aExtra, 1, GetTime() ));
		return 0;
	}
	if(idID == TABL)
	{
		if(ExtraGetValue(aExtra, 1))
			return Action_UseGrabWait(pItem, 35, aExtra);
		pItem->ExecActivity(this);
		return 0;
		pos = [pItem->GetX(), pItem->GetY(), 0, ];
		if(Contained() != pItem && ExecMoveTo(pos[0], pos[1], pos[2]))
		{
			pItem->ExecActivity(this);
			return 0;
		}
		return 0;
		if(iActItemState == 0)
		{
			iActItemState = pItem->Contents(Random(pItem->ContentsCount()));
			if(!iActItemState) return 1;
		}
		if(GetType(iActItemState) == C4V_C4Object)
		{
			var pos = pItem->GetPos(iActItemState);
			if(ExecMoveTo(pos[0], pos[1], pos[2], pos[3]))
			{
				if(iActItemState->~IsFood())
					iActItemState->~Eat(this);
				else if(iActItemState->~IsDrink())
					iActItemState->~Drink(this);
				var aMessages = ["hmm", "mjam", "njm"];
				MessageEx(aMessages[Random(GetLength(aMessages))], this);				
				AddEffect("IntWait", this, 1, 35*4);
				iActItemState = 0;
				return 0;
			}
			return 0;
		}
		else iActItemState = 0;
		return 0;
	}
	return _inherited(pItem, aExtra, ...);
}

func StopActivityItem(pItem, aExtra)
{
	var idID = NONE;
	if(GetType(pItem) == C4V_C4Object) idID = GetID(pItem);
	if(GetType(pItem) == C4V_C4ID) idID = pItem;
	if(idID == ARCH) // Process
  {
		pItem->CancelActivity(this);
		return 0;
  }
  if(idID == TABL) // Process
  {
		pItem->CancelActivity(this);
		return 0;
  }
	return _inherited(pItem, aExtra, ...);
}