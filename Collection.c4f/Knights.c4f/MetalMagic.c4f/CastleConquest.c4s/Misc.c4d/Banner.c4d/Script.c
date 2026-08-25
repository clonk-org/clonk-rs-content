/*-- Fahne --*/

#strict 2

protected func Initialize()
{
  return 1;
}

/* TimerCall */

private func Wind()
{
    //Richtung der Windgeschwindigkeit anpassen
    if(GetAction() == "FlyBuilding") SetDir(BoundBy(GetWind() / 15 + 3, 0, 6));
}

protected func FlyCrew()
{
 SetDir(6-BoundBy(GetXDir(GetActionTarget())/3+3,0,6));
}

protected func AttachTargetLost()
{
RemoveObject();
}

protected func Entrance(pObj)
{
 var flag=FindContents(FLGP,GetActionTarget());
 if(!flag)RemoveObject();
 
 if(! (pObj->~IsStaircase()))
 flag->HasEntered();
}

protected func EndCallDown()
{
 GetActionTarget()->~Down();
 RemoveObject();
}
