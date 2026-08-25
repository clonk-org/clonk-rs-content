/*-- Holzhütte --*/

#strict 2

#include DOOR

local pPartner;

func IsDoor() { return 1; }

func GetWait() { return 35; }

func SetTarget(pTarget, iX, iY)
{
//  if(ObjectCount(GetID())%2 == 0) return;
//  var pTarget = FindObject2(Find_ID(B_C1, 0, 0, -1, -1));
  if(LocalN("pOutside", pTarget)) pTarget = LocalN("pOutside", pTarget);
  SetAction("DoorClosed", pTarget);
  
  pPartner = CreateObject(GetID());
  pPartner->SetAction("DoorClosed", LocalN("pInside", pTarget));
  pPartner->LocalN("pPartner") = this;
  pPartner->SetCategory(C4D_Foreground);
  pPartner->SetClrModulation(RGBa(255,255,255,100));
}

local fEnter;

func SoundOpenDoor()
{
  if(fEnter) return _inherited(...);
  fEnter = 1;
  pPartner->SetAction("OpenDoor");
  fEnter = 0;
  return _inherited(...);
}

func SoundCloseDoor()
{
  if(fEnter) return _inherited(...);
  fEnter = 1;
  pPartner->SetAction("CloseDoor");
  fEnter = 0;
  return _inherited(...);
}


protected func Collection2(pObj)
{
  if(GetProcedure(pObj) == "ATTACH" && GetOCF(pObj->GetActionTarget()) & OCF_CrewMember && GetPlayerType(GetOwner(pObj->GetActionTarget())) == C4PT_User)
    return;
  if(pPartner && !fEnter)
  {
    var pLayer = GetObjectLayer();
    if(pLayer) pLayer->RemObject(pObj);
    pPartner->DoEnter(pObj);
  }
}

public func DoEnter(pObj)
{
  fEnter = 1;
  Enter(this(), pObj);
  fEnter = 0;
  var pLayer = GetObjectLayer();
  if(pLayer) pLayer->AddObject(pObj);
  SetCommand(pObj, "Exit");
}

protected func ActivateEntrance(pObj)
{
  if (GetAction() == "DoorClosed" || ActIdle())
  {
    SetAction("OpenDoor");
    pPartner->SetAction("OpenDoor");
  }
  return 1;
}