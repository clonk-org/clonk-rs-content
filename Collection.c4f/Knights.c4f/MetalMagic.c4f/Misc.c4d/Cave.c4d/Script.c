/*-- Höhle --*/

#strict 2

local pLink;

protected func Initialize()
{ 
  SetEntrance(1);   
}

private func CheckContents()
{
  var pExit;
  if(GetType(pLink) == C4V_Array)
  {
    pExit = pLink[Random(GetLength(pLink))];
  }
  else pExit = pLink;
  if(!pExit) pExit = FindOtherExit();
  if(!pExit) return;
  var obj;
  while(obj = Contents()) 
  {  //Enter(pExit, obj);
     Exit(Contents(), GetX(pExit)-GetX(),GetY(pExit)-GetY()+15,0, 0);
     //SetCommand(obj, "Exit");   
  }
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
