/*-- Stuhl --*/

#strict 

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetDir(DIR_Left());
  if (Random(2)) SetDir(DIR_Right());
  SetComDir(COMD_Stop());
  return(1);
}

//Aufsitzen
protected func ActivateEntrance(pClonk)
{
  if (GetRider()) return(0); //Immer nur einer ;)
  // Rauf
  if (!ObjectSetAction(pClonk,"Ride",this()))
    return(0);
  return(1);
}

public func ControlDown(pByObject)
{
  // Reiter: absitzen
  if (pByObject == GetRider())
    { ObjectSetAction(pByObject,"Walk"); SetActionTargets(0,0,pByObject); return(1); }
  // Keine Steuerung
  return(0);
}

public func ControlUp(pByObject)
{
  if(!GetRider()) {return(0);} //Nur mit Reiter, sonst schieben
  return(1);              
}

public func ControlDigDouble(pClonk)
{
  Exit(Contents(), 0,20, 0, 0, 1);
  return(1);
}


/* Status */

public func GetRider()
{
  var pRider;
  if(pRider = FindObject2(Find_OCF(OCF_CrewMember), Find_ActionTarget(this), Find_Func("IsRiding")))
     return(pRider);
}

