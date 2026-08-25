#strict 2
#include TAHT

func Initialize()
{
	_inherited(...);
	SetEntrance(1);
}

public func GetEntranceZoneCount() { return 3; }

public func GetEntranceZone(fInside, iIndex)
{
  if(iIndex == 0)
  {
    if(fInside) return Find_InRect(98-140,57-154,30,49); // Jeweils Offset und Größe
    return Find_InRect(50-140,59-154,42,58);
  }
  if(iIndex == 1)
  {
    if(fInside) return Find_InRect(134-140,82-154,32,43); // Jeweils Offset und Größe
    return Find_InRect(173-140,83-154,32,42);
  }
  if(iIndex == 2)
  {
    if(fInside) return Find_InRect(266-140,268-154,31,41); // Jeweils Offset und Größe
    return Find_InRect(234-140,273-154,24,36);
  }
}

public func GetEntranceDir(fInside, iIndex)
{
  if(iIndex==0)
  {
    if(fInside) return COMD_Right;
    return COMD_Left;
  }
  if(iIndex==1)
  {
    if(fInside) return COMD_Left;
    return COMD_Right;
  }
  if(iIndex==2)
  {
    if(fInside) return COMD_Right;
    return COMD_Left;
  }
}