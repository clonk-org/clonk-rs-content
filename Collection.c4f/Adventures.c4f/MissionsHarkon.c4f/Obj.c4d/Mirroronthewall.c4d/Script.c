/*-- Spieglein an der Wand --*/

#strict

local pLink,bOn;

protected func Initialize()
{ 
  bOn=0;
  SetEntrance(0); 
  SetAction("Empty");  
}

protected func ActivateEntrance(pClonk)
{
  if(!(GetEffect("WipfSoap",pClonk)))
  {
    FinishCommand(pClonk, 1);
    //AddCommand(pClonk, "Jump");
    return(Message("Du stinkst!",this()));
  }
  if(!(GetEffect("LovePotion",pClonk)))
  {
    FinishCommand(pClonk, 1);
    //AddCommand(pClonk, "Jump");
    return(Message("Du riechst wie ein Aquaclonk!",this()));
  }
  Enter(this, pClonk);
}

func Activate()
{
   bOn=!bOn;
   
  if(bOn)
  { 
   // erscheinen
   SetAction("Appear");
  }
  else
  SetAction("Disappear");
  return(1);
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
  if(!pExit) return();
  var obj;
  while(obj = Contents()) 
  {  //Enter(pExit, obj);
     Exit(Contents(), GetX(pExit)-GetX(),GetY(pExit)-GetY()+15,0, 0);
     //SetCommand(obj, "Exit");   
  }
 // erscheinen
if(!bOn)
  {
  if(FindObject2(Find_Distance(100), Find_OCF(OCF_CrewMember)))
  Activate(this);
  return(1);
  }

  if(bOn)
  {
   if (!FindObject2(Find_Distance(100), Find_OCF(OCF_CrewMember)))
    {
    Activate(this);
    return(1);
    }
  }
  return(1);
}

private func FindOtherExit()
{
  var obj, count;
  while(obj = FindObject(GetID(), 0, 0, 0, 0, OCF_Entrance, 0, 0, 0, obj)) 
    if(obj != this()) // Nicht sich selbst
      if(!LocalN("pLink", obj)) // Keines, das schon einen Ausgang hat
        { Var(count++) = obj; }
  return(Var(Random(count)));
}
