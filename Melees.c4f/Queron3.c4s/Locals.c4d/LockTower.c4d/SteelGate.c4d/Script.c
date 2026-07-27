/*-- Wachturm-Tor --*/

#strict 2

local fOpen;

protected func Initialize()
	{
	fOpen = 0;
	SetAction("Gate");
	}

public func Activate()
	{
  Sound("Click");
	if (fOpen) return Close();
	else return Open();
	}

private func Open()
	{
	if (fOpen) return;
	Sound("SteelGate1");
	SetComDir(COMD_Up);
	fOpen = 1;
	return 1;
	}
	
private func Close()
	{
	if (!fOpen) return;
	Sound("SteelGate1");
	SetComDir(COMD_Down);
	fOpen = 0;
	return 1;
	}

public func IsOpen() { return fOpen; }
