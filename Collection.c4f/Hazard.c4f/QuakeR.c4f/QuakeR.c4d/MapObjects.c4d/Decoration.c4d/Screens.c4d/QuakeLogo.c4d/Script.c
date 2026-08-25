/*-- Quake Logos --*/

#strict

local Step;

func Initialize()
{
	Step = Random(3)+1;
	SetAction(Format("Show%d",Step));
}

func NextOne()
{
	Step++;
	if(Step > 3)
		Step = 1;
	
	Schedule(Format("SetAction(\"Show%d\")",Step), 75, 0, this());
}
