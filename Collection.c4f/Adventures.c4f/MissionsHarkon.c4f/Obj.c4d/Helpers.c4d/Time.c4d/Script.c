/*-- Uhrzeit --*/

#strict 2

private func DayLength() { return 3000; }

local iTime;
local iHour;
local iMinute;
local fStop;

protected func Initialize()
{
  SetAction("Initialize");
	DoSetTime(12,00);
  return 1;
}

global func GetTime(fMinutes)
{
	var pObj = FindObject2(Find_ID(_TME));
	if(!pObj) pObj = CreateObject(_TME, 0, 0);
	return pObj->DoGetTime(fMinutes);
}

global func StopTime(fRun)
{
  var pObj = FindObject2(Find_ID(_TME));
  if(!pObj) pObj = CreateObject(_TME, 0, 0);
  return pObj->DoStopTime(fRun);
}

func DoStopTime(fRun) { if(fRun) fStop = 0; else fStop = 1; }

func DoGetTime(fMinutes)
{
	if(fMinutes)
		return iMinute;
	return iHour;
}

// How many frames untill the specified time?
global func GetDurationToTime(iNewHour, iNewMinute)
{
	var pObj = FindObject2(Find_ID(_TME));
	if(!pObj) pObj = CreateObject(_TME, 0, 0);
	return pObj->DoGetDurationToTime(iNewHour, iNewMinute);
}

func DoGetDurationToTime(iNewHour, iNewMinute)
{
	var iNewTime = iNewHour*DayLength()/24+iNewMinute*DayLength()/24/60;
	var diff = iNewTime-iTime;
	if(diff < 0) diff += DayLength();
	if(diff >= DayLength()) diff -= DayLength();
	return diff*10;
}

func Timer()
{
  if(fStop)
    return;
	iTime++;
	if(iTime >= DayLength())
		iTime = 0;
	iHour = iTime*24/DayLength();
	iMinute = (iTime-iHour*DayLength()/24)*24*60/DayLength();
//	Message("%2d:%02d", 0, iHour, iMinute);
}

global func SetTime(iNewHour, iNewMinute)
{
	var pObj = FindObject2(Find_ID(_TME));
	if(!pObj) pObj = CreateObject(_TME, 0, 0);
	return pObj->DoSetTime(iNewHour, iNewMinute);
}

func DoSetTime(iNewHour, iNewMinute)
{
	iTime = iNewHour*DayLength()/24+iNewMinute*DayLength()/24/60;
}