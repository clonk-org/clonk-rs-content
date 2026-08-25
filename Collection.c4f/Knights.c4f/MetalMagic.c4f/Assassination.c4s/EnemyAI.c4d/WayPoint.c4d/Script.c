/*---- Punkt ----*/

#strict 2

local aWay;
local aContainer;
local aCommand;
local aDoor;

protected func Initialize()
{
  SetAction("Exist");
  SetVisibility(VIS_God);
  aWay = aContainer = aCommand = aDoor = [];
  return 1;
}

public func GetRandomWay(aUseNot)
{
  var aPossibleWays = aWay;
  var i;
  if(aUseNot)
  for(var pWay in aWay)
  {
    for(var pTest in aUseNot)
    {
      if(pTest == pWay)
      {
        DeleteArrayItem(i, aPossibleWays);
        i--;
        break;
      }
    }
    i++;
  }
  if(GetLength(aPossibleWays)==0) return -1;
  var pChosen = aPossibleWays[Random(GetLength(aPossibleWays))];
  i = 0;
  for(var pTest in aWay)
  {
    if(pChosen == pTest) return i;
    i++;
  }
  return -1;
}

public func ConnectUp(fNoBack)
{
  var pObj, pStairs, szCommand;
  while(pObj = FindObject(GetID(), 0, 0, -1, -1, 0, 0, 0, 0, pObj))
    if(GetY(pObj)<GetY() && Abs(GetX()-GetX(pObj))<10)
    {
      if(Abs(GetY()-GetY(pObj))>100) return;
      pStairs = FindObject2(Find_Func("IsStaircase"), Find_AtPoint());
      if(pStairs) szCommand = "ContainedUp";
      SetConnection(pObj, pStairs, szCommand);
      if(!fNoBack) pObj->~ConnectDown(1);
      return true;
    }
}

public func ConnectDown(fNoBack)
{
  var pObj, pStairs, szCommand;
  while(pObj = FindObject(GetID(), 0, 0, -1, -1, 0, 0, 0, 0, pObj))
    if(GetY(pObj)>GetY() && Abs(GetX()-GetX(pObj))<10)
    {
      if(Abs(GetY()-GetY(pObj))>100) return;
      pStairs = FindObject2(Find_Func("IsStaircase"), Find_AtPoint());
      if(pStairs) szCommand = "ContainedDown";
      SetConnection(pObj, pStairs, szCommand);
      if(!fNoBack) pObj->~ConnectUp(1);
      return true;
    }
}

public func ConnectLeft(fNoBack)
{
  var pObj;
  while(pObj = FindObject(GetID(), 0, 0, -1, -1, 0, 0, 0, 0, pObj))
    if(GetX(pObj)<GetX() && Abs(GetY()-GetY(pObj))<50)
    {
 //     var iPath = GetPathLength (GetX(), GetY(), GetX(pObj), GetY(pObj));
 //     if(!iPath || iPath>ObjectDistance(pObj)+30) return;
      var pDoor;
      var pTower = FindObject2(Find_ID(CPT2), Find_AtPoint());
      if(pTower) pDoor = pTower->FindGateLeft();
      SetConnection(pObj, 0, 0, pDoor);
      if(!fNoBack) pObj->~ConnectRight(1);
      return true;
    }
}

public func ConnectRight(fNoBack)
{
  var pObj;
  while(pObj = FindObject(GetID(), 0, 0, -1, -1, 0, 0, 0, 0, pObj))
    if(GetX(pObj)>GetX() && Abs(GetY()-GetY(pObj))<50)
    {
 //     var iPath = GetPathLength (GetX(), GetY(), GetX(pObj), GetY(pObj));
 //     if(!iPath || iPath>ObjectDistance(pObj)+30) return;
      var pDoor;
      var pTower = FindObject2(Find_ID(CPT2), Find_AtPoint());
      if(pTower) pDoor = pTower->FindGateRight();
      SetConnection(pObj, 0, 0, pDoor);
      if(!fNoBack) pObj->~ConnectLeft(1);
      return true;
    }
}

public func DoConnections()
{
  return ConnectUp()+ConnectDown()+ConnectLeft()+ConnectRight();
}

public func SetConnection(pTarget, pContainer, szCommand, pDoor, fNoEffect)
{
  if(pTarget==this) return;
  for(var pTest in aWay) if(pTest == pTarget) return;
  var i = GetLength(aWay);
  aWay[i] = pTarget;
  aContainer[i] = pContainer;
  aCommand[i] = szCommand;
  aDoor[i] = pDoor;
  if(!fNoEffect) DrawParticleLine("NoGravSpark", 0, 0, GetX(pTarget)-GetX(), GetY(pTarget)-GetY(), 5, 50, RGB(255), RGB(0,255));
  return pTarget;
}

public func GetConnected(iIndex, iItem)
{
  if(iItem==0) return aWay[iIndex];
  if(iItem==1) return aContainer[iIndex];
  if(iItem==2) return aCommand[iIndex];
}

public func DoMoveTo(pTarget, pObj)
{
  var i;
  for(var pTest in aWay)
  {
    if(pTest == pTarget) break;
    i++;
  }
  if(i>=GetLength(aWay) || !pObj) return;
  AddCommand(pObj, "MoveTo", pTarget);
  if(aCommand[i]) AddCommand(pObj, "Call", aContainer[i], pObj, 0, 0, 0, aCommand[i]);
  if(aContainer[i]) AddCommand(pObj, "Enter", aContainer[i]);
}

public func ResetPoints()
{
  for(var pObj in FindObjects(Find_ID(GetID()))) pObj->Initialize();
  for(var pObj in FindObjects(Find_ID(GetID()))) pObj->DoConnections();
}
