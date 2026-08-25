/*-- B‰ume werden zu Holz --*/

#strict

#appendto TREE

public func ChopDown()
{
  // Ger‰usch
  if(GetCon() > 50) Sound("TreeDown*");
  // Command der f‰llenden Clonks abschlieﬂen
  var pObj;
  while(pObj = FindObject(,,,,,, "Chop", this(),, pObj))
    FinishCommand(pObj, true);
  // Zu Holz zerfallen
  Split2Components();
  // Fertig
  return(1);
}

