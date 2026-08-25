/*-- Höhle --*/

#strict 2

local pLink;

protected func Initialize()
{ 
  SetEntrance(1);   
}

func DoExit(pClonk)
{
	var pTarget = pClonk->Contained();
	var iXOffset = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 0);
	var iYOffset = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 1);
	var iWidth   = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 2);
	var iHeight  = GetDefCoreVal("Entrance", "DefCore", GetID(pTarget), 3);
	var iX = iXOffset+iWidth/2;
	var iY = iYOffset+iHeight;

	Exit(pClonk, iX, iY);
	return 1;
}

public func GetLink() { return pLink; }

func IsDoor() { return 1; }

func GetWait() { return 35; }

local fEnter;

protected func Collection2(pObj)
{
	if(pLink && !fEnter)
	{
		var pLayer = GetObjectLayer();
		if(pLayer) pLayer->RemObject(pObj);
		pLink->DoEnter(pObj);
	}
}

public func DoEnter(pObj)
{
	var pLayer = GetObjectLayer();
	if(pLayer) pLayer->AddObject(pObj);
  fEnter = 1;
  Enter(this, pObj);
  fEnter = 0;
	DoExit(pObj);
//  SetCommand(pObj, "Exit");
}

protected func ActivateEntrance(pObj)
{
  if (GetAction() == "DoorClosed" || ActIdle()) SetAction("OpenDoor");
  return 1;
}

private func FindOtherExit()
{
  var obj, count;
  while(obj = FindObject(GetID(), 0, 0, 0, 0, OCF_Entrance, 0, 0, 0, obj)) 
    if(obj != this) // Nicht sich selbst
      if(!LocalN("pLink", obj)) // Keines, das schon einen Ausgang hat
        { Var(count++) = obj; }
  return Var(Random(count));
}
