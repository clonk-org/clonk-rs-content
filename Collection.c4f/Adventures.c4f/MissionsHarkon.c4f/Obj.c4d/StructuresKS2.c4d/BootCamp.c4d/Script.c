/*-- Ausbildungslager --*/

#strict 2
#include B100

local ascend;

func Initialize() {
  SetEntrance(1);
  ascend = CreateObject(K_UP, -29, -9, GetOwner());
  ascend->Activate(this(), false);
  UpdateTransferZone();
  return(1);
}

public func ContainedRight(pClonk)
{
  [$Ascend$|Image=ASCN:0]
  if (!Hostile(GetOwner(), pClonk->GetOwner()))
    Exit(pClonk, -19, -12);
  return(1);
}

protected func ContextAscend(object pClonk)
{
  [$Ascend$|Image=ASCN:0|Condition=IsComplete]
  pClonk->ContainedCall("ContainedRight", this());
}

private func IsComplete()
{
  return (GetCon() >= 100); 
}


protected func UpdateTransferZone()
{
  SetTransferZone(-50, -45, 85, 120);
}


protected func Incineration()
{
  // Niedergang
  if (ascend) RemoveObject(ascend);
  return(_inherited() );
}

protected func Wood()
{
  SetGraphics("Wood",0);
}
