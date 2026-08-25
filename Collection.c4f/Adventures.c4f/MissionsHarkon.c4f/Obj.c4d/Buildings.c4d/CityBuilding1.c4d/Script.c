#strict 2
#include TAHT

func Initialize()
{
	_inherited(...);
	SetEntrance(1);
}

public func GetEntranceZone(fInside)
{
  if(fInside) return Find_InRect(42-125,33-92,17,27); // Jeweils Offset und Größe
  return Find_InRect(59-125,37-92,29,23);
}

public func GetEntranceZoneCount() { return 1; }

public func GetEntranceDir(fInside)
{
	if(fInside) return COMD_Left;
	return COMD_Right;
}

public func DoSmoke()
{
	if(pOutside) return pOutside->DoSmoke();
	Smoke(53, -35+10, 20);
}