/*-- Superkuppel Unterwasser --*/
/
#strict
/* Verbuggt
#appendto CNKT

public func Activate(pClonk)
{ 
  [$TxtCreateconstructionsi$]
  SetComDir(COMD_Stop(), pClonk);
  if (pClonk && !GetPhysical("CanConstruct", PHYS_Current, pClonk))
  {// Falls der Clonk nicht bauen kann 
    PlayerMessage(GetController(pClonk), "$TxtCantConstruct$", this, GetName(pClonk)); 
    return false;
  }// Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN, pClonk, this(), 1, "$TxtNoconstructionplansa$");
  var idType; var i = 0;
  while (idType = GetPlrKnowledge(GetOwner(pClonk), 0, i++, C4D_Structure()))
  {// Baupläne des Spielers durchgehen
    if(idType==SPDM && GetMaterial(GetX(pClonk),GetY(pClonk))!=Material("Water"))
      {Log("Kein Wasser");return 1;}
    //if(idType==SPDM && GetY(pClonk)<333)
    //  {Log("Zu hoch");return 1;}
    if(idType->~IsConkitBuilding() || (!idType->~IsIndianHandcraft() && !idType->~IsTrapperHandcraft()))
      AddMenuItem("$TxtConstructions$", "CreateConstructionSite", idType, pClonk);
  }
  return(1);
}
