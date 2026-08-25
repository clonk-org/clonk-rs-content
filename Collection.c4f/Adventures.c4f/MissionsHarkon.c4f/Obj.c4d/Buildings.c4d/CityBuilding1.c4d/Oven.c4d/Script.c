#strict 2

func Initialize()
{
	var pTarget = FindObject2(Find_ID(B_C1, 0, 0, -1, -1));
	if(LocalN("pInside", pTarget)) pTarget = LocalN("pInside", pTarget);
	SetAction("Burn", pTarget);
}

func DoSmoke()
{
	GetActionTarget()->DoSmoke();
}