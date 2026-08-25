/*-- U-Boot appendto--*/

#strict

#appendto SUB1

/* Einsammeln & Eingang */

protected func RejectCollect(id idObject, object pObject)
{
  // Anderes Objekt nicht im Freien
  if (Contained(pObject)) return(0);
  // Torpedotreffer
  if (pObject->~IsTorpedo())
    return(TorpedoHit(pObject));
  // U-Boot sammelt an Land nichts ein
  if (GetAction() ne "Swim") return(1);
  // Bei bewegten Objekten Hit auslösen (Sprengpfeil / Flint explodiert zB)
  if(Abs(GetXDir(pObject))>25 || Abs(GetYDir(pObject))>25) ProtectedCall(pObject, "Hit");
  // Objekt befindet sich nicht im Bereich des Greifers
  if (!Inside(GetX(pObject) - GetX() + 30 - GetDir() * 60, -8, +8)) return(1);
  if (!Inside(GetY(pObject) - GetY() - 14, -8, +8)) return(1);
  // Sonst Einsammeln okay
  return(0);
}

private func TorpedoHit(object pTorpedo)
{
  if(GetAction(pTorpedo) ne "Swim") return(1);
  if(GetActTime(pTorpedo) < 25) return(1);
  pTorpedo->~Hit();
  return(1);
}

// Kann nicht in Gebäude gebracht werden

protected func RejectEntrance(object pIntoObj) {return(1);}