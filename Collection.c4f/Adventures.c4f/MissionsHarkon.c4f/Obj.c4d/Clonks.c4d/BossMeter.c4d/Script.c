/*-- Baum --*/

#strict 2

local iLength;
local iPercent;
local init;

func SetLength(iNew)
{
	iLength = BoundBy(iNew, 10, 10000);
	UpdateGraphics();
}

func SetPercent(iNew)
{
	iPercent = BoundBy(iNew, 0, 100);
	UpdateGraphics();
}

func UpdateGraphics()
{
	SetShape(0, -5, iLength, 10);
	SetObjDrawTransform(100*iLength, 0, 0, 0, 1000, 0, this, 1);
	SetObjDrawTransform(Min(iPercent,init)*iLength, 0, 0, 0, 1000, 0, this, 2);
}

func DoInit()
{
   if(init == 100) return;
   init++;
   ScheduleCall(this, "DoInit", 1,1);
   UpdateGraphics();
}

func Initialize()
{
	iLength = 500;
	iPercent = 100;
	init = 0;
	ScheduleCall(this, "DoInit", 1,1);
	SetGraphics("", this, GetID(), 1, 1);
	SetGraphics("Full", this, GetID(), 2, 1);
	UpdateGraphics();
}
