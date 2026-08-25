/*-- Unsichtbarkeit --*/

#strict

#appendto MINV

public func Activate(object pCaster, object pCaster2)
  {
  // Zauberer ermitteln
  if (pCaster2) pCaster = pCaster2;
  // Magie kann man hören, ganz klar ;)
  Sound("Magic*");
  // Wenn Objektkomponente vorhanden, mehrere Objekte unsichtbar machen
  var pCom=FindContents(LNKT,pCaster),pObj,iCnt;
  // Objektkomponente vorhanden
  if(pCom) {
    // alle Objekte in der Nähe zur Unsichtbarmachung in Erwägung ziehen
    while(pObj=FindObject(0,0,0,-1,-1,0,0,0,NoContainer(),pObj)) {
      // nur Freunde (außer wenn eine bestimmte Regel aktiviert ist)
      if(Hostile(GetOwner(pObj),GetOwner(pCaster)) && !ObjectCount(NTMG)) continue;
      // nur Objekte die einen Besitzer haben
      if(GetOwner(pObj)==-1) continue;
      // Objekt zu weit weg oder schon genug Objekte unsichtbar gemacht 
      if(ObjectDistance(pObj,pCaster)>100 || (iCnt++)>RandomX(3,5)) break;
      AddEffect("InvisPSpell",pObj,1400,1111,0,GetID());
    }
  } 
  if(iCnt>1) RemoveObject(pCom);
  // Zauberer unsichtbar machen (40sec)
  else 
  {
    AddEffect("InvisPSpell", pCaster, 200, 1400, 0, GetID());
    // Schild auch unsichtbar machen
    if(pCaster->~IsKnight())
      if(pCaster->HasShield())
        AddEffect("InvisPSpell", pCaster->HasShield(), 200, 1400, 0, GetID());
  }
  // Fertig - das Zauberobjekt wird nun nicht mehr gebraucht
  return(RemoveObject());
  }

