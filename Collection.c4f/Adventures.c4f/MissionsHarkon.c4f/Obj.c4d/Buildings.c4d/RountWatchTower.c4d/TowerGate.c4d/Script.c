#strict 2

func Initialize()
{
	var pTarget = FindObject2(Find_ID(B_RW));
	if(LocalN("pMain", pTarget)) pTarget = LocalN("pMain", pTarget);
	SetAction("DoorClosed", pTarget);
}

func ControlLeft()
{
	if(GetAction() == "DoorClosed")
		return SetAction("OpenDoor");
	if(GetAction() == "CloseDoor");
	{
		var iPhase = GetPhase();
		SetAction("OpenDoor");
		SetPhase(12-iPhase);
		return;
	}
}

func ControlRight()
{
	if(GetAction() == "DoorOpen")
		return SetAction("CloseDoor");
	if(GetAction() == "OpenDoor");
	{
		var iPhase = GetPhase();
		SetAction("CloseDoor");
		SetPhase(12-iPhase);
		return;
	}
}

func OpenEntrance() { SetSolidMask(); }
func CloseEntrance() { SetSolidMask(35,0,8,57,35); }