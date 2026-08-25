/*-- Lebensabläufe --*/

#strict 2

#appendto CLNK

local aZeiten;

local aWorkActivitys;
local aLeisureActivitys;
local aSleepActivitys;

func InitializeLife()
{
	// Arbeit, Freizeit, Schlafen
	aZeiten = [8,16,23];
}

func GetLifeState(DoLog)
{
	if(!aZeiten) return 0;//InitializeLife();
	var aTimes = CreateArray(24);
	var aZeiten2 = aZeiten;
	aZeiten2[3] = aZeiten2[0];
	for(var j = 0; j < 3; j++)
	{
		if(aZeiten2[j+1]<aZeiten2[j]) aZeiten2[j+1] += 24;
		for(var i = aZeiten2[j]; i < aZeiten2[j+1]; i++)
			aTimes[i%24] = j;
	}
//	Message("%s", this, GetName());
	if(DoLog) Log("%v", aTimes);
	return aTimes[GetTime()];
	var j = 0;
	for(var i = 0; i < 24; i++)
	{
		if(j == 3)
		{
			aTimes[i] = 0;
			continue;
		}
		if(i >= aZeiten[j])
			j += 1;
		aTimes[i] = j;
	}
	var state = aTimes[GetTime()];
	if(state == 0) state = GetLength(aZeiten)-1;
	else state -= 1;
	Message("%s", this, GetName());
	if(DoLog) Log("%v", aTimes);
	return state;
	
	
	var state = 0;
	var iTime = GetTime();
	var j = 0;
	for(var i = 0; i < GetLength(aZeiten); i++)
	{
		j = i-1;
		if(j == -1) j += 3;
		if(aZeiten[j] == 24 && aZeiten[i] == 0) continue;
		if(Inside(iTime, aZeiten[j], aZeiten[i]) || Inside(iTime+24, aZeiten[j], aZeiten[i]+24))
		{
			state = i;
			break;
		}
	}
	if(state == 0) state = GetLength(aZeiten)-1;
	else state -= 1;
	Message("%s", this, GetName());
	return state;
}

func GetDefaultLeisure()
{	return [[LOOK]];
	var pArchery = FindObject(ARCH);
	if(pArchery->FreePlace() && CanUse(BOW1))
		return [[pArchery]];
	return [[LOOK]];
}

func GetDefaultSleep()
{
	return [[LOOK]];	
	var pObj = FindObject2(Find_ID(BED1), Find_Func("IsFree"), Sort_Distance());
	return [[pObj]];
}

func GetNewActivity()
{
	if(GetLifeState() == 0) // Arbeiten
	{
		return GetActivityFromList(aListActivitys);
	}
	else if(GetLifeState() == 1) // Freizeit
	{
		if(aLeisureActivitys)
			return GetActivityFromList(aLeisureActivitys);
		else
			return GetActivityFromList(GetDefaultLeisure());
	}
	else if(GetLifeState() == 2) // Schlafen
	{
		if(aSleepActivitys)
			return GetActivityFromList(aSleepActivitys);
		else
			return GetActivityFromList(GetDefaultSleep());
	}
}

func GetActivityFromList(aListActivitys)
{
	if(!aListActivitys) return 0;
	var list = aListActivitys[Random(GetLength(aListActivitys))];
	for(var pItem in list)
	{
		var aExtra = [];
		if(GetType(pItem) == C4V_Array)
		{
			aExtra = pItem;
			pItem = pItem[0];
		}
		if( !(TestActivityPossible(pItem, aExtra) ) ) return 0;
	}
	for(var pItem in list)
		PushBack(pItem, iCurrentActivity);
	return 1;
}