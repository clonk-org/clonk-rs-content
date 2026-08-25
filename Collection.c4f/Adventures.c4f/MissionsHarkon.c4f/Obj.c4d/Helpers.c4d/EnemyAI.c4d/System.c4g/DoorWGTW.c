/* Türen */

#strict 2

// Lagerturm
#appendto WGTW

local fCounterR;
local fCounterL;

func KIOpen(pClonk, dir)
{
	if(dir == COMD_Right)
	{
		fCounterR++;
		if(!RightGateOpen()) ControlRight(pClonk);
	}
	else
	{
		fCounterL++;
		if(!LeftGateOpen()) ControlLeft(pClonk);
	}
}

func KIClose(pClonk, dir)
{
	if(dir == COMD_Right)
	{
		fCounterR--;
		if(fCounterR <= 0) if(RightGateOpen()) ControlRight(pClonk);
	}
	else
	{
		fCounterL--;
		if(fCounterL <= 0) if(LeftGateOpen()) ControlLeft(pClonk);
	}
}