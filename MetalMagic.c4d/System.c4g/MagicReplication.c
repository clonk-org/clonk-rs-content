/*-- Magische Kopie --*/

#strict 2

#appendto MGRP

private func CreateObject(idID, iX, iY, iOwner)
{
  var pObj = _inherited(idID, iX, iY, iOwner);
  // Keinen Stab für kopierte Magi
  if(ObjectCount(FSTF))
   if(FindContents(SCKZ, pObj)) RemoveObject(FindContents(SCKZ, pObj));
  return pObj;
}
