/*-- Feuerschuss --*/

#strict

#include _FSH

func Timer() { // alle 40 Frames
  var iXDir,iYDir,iAngle;
  var pActTarget = GetActionTarget();
  // Kein Feuer-Spucker: löschen
  if(!pActTarget) return(RemoveObject());

  // In Material, was Feuer löscht (Wasser): löschen
  if(GetMaterialVal("Extinguisher","Material",GetMaterial(0,0))) return(Sound("Pshshsh"),RemoveObject());
  // löschen
  if(GetActTime()>iLifetime) return(RemoveObject());

  // Pseudo-Attach
  SetPosition(GetX(pActTarget),GetY(pActTarget));

  // Größe+Drehung des Zielobjektes in die Position einbeziehen
  var iX2 = (Cos(GetR(pActTarget), iX*GetCon(pActTarget)) - Sin(GetR(pActTarget), iY*GetCon(pActTarget)))/100;
  var iY2 = (Sin(GetR(pActTarget), iX*GetCon(pActTarget)) + Cos(GetR(pActTarget), iY*GetCon(pActTarget)))/100;

  // Optionales Vertex-Attachment am Trägerobjekt (überschreibt vorhergehende Ausrichtungsfunktionen)
  if (fVertexAttach)
  {
    iX2 = pActTarget->GetVertex(iAttachVertex, 0);
    iY2 = pActTarget->GetVertex(iAttachVertex, 1);
  }

  for(var i=iSpeed/60+1; i>0 ;--i) {

    iAngle=RandomX(-12,+12);

    // Winkel usw berechnen
    if(!Random(3)) iAngle=RandomX(-12,+12);
    iXDir=Sin(GetR()+iAngle,iSpeed) + GetXDir(GetActionTarget());
    iYDir=-Cos(GetR()+iAngle,iSpeed) + GetYDir(GetActionTarget());
    
    // Effekt
    CreateParticle("DBFire",iX2,iY2,iXDir,iYDir,RandomX(50,100),RGB(255,255,255));	 

    // Anzünd-Objekte in die Richtung verschießen
    CreateObject(_FBU,
		 iX2+Sin(GetR()+iAngle,10),
		 iY2-Cos(GetR()+iAngle,18),
		 GetOwner())->ActivateInflamer(iXDir, iYDir, fLethal);
  }
}

// Globale Funktion zum Erstellen
global func CreateFireBurst(object pCaster, int iTime, int iX, int iY, int iRot, int iRotSpeed, int iExtraSpeed)
  {
  return(CreateObject(_FBU,0,0,GetOwner(pCaster))->_FBU::Activate(pCaster,iTime, iX, iY, iRot, iRotSpeed, iExtraSpeed));
  }