/*-- Truhe --*/

#strict

protected func Initialize()
{
  SetAction("Closed");
  return(1);
}
  
protected func ChestSound() { Sound("ChestOpening"); }

protected func RejectCollect(id idObj, object pObj)
{
  // Gegenstände nur annehmen, wenn die Truhe offen ist
  return(GetAction() ne "Open");
}
  
protected func ControlUp(object pClonk)
{
  [$DescOpen$]
  if(GetAction() eq "Closed")
    SetAction("Opening");
}
  
protected func ControlThrow(object pClonk)
{
  [$DescTake$]
  if(GetAction() ne "Open") { return(true, ControlUp(pClonk)); }
  // Keine Überladung: Normales Nehmen ausführen
}

protected func ControlLeft(object pClonk)
{
  [$DescTake$]
  // Nicht offen? -> Zuerst öffnen
  if(GetAction() ne "Open") { return(true, ControlUp(pClonk)); }
  // Clonk darf herausnehmen
  SetCommand(pClonk, "Activate", 0, 0, 0, this());
  return(true);
}

protected func ControlRight(object pClonk)
{
  [$DescTake$]
  // Nicht offen? -> Zuerst öffnen
  if(GetAction() ne "Open") { return(true, ControlUp(pClonk)); }
  // Clonk darf herausnehmen
  SetCommand(pClonk, "Activate", 0, 0, 0, this());
  return(true);
}

protected func ControlDig(object pClonk)
{
  [$DescTake$]
  // Nicht offen? -> Zuerst öffnen
  if(GetAction() ne "Open") { return(true, ControlUp(pClonk)); }
  // Clonk darf herausnehmen
  SetCommand(pClonk, "Activate", 0, 0, 0, this());
  return(true);
}
  
protected func ControlDownSingle(object pClonk)
{
  [$DescClose$]
  if(GetAction() eq "Open") return(SetAction("Closing"));
}
  
