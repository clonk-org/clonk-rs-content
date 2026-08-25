/*-- Gasschuss --*/

#strict

local iX,iY,pTarget,iLifetime,iSpeed,fLethal;
local fVertexAttach, iAttachVertex;

static const FBRT_BaseSpeed = 30;

// "Konstruktor"
func Activate(pTrg,iTime,iXPos,iYPos,iAngle,iAngleDir,int iExtraSpeed, bool fDeadly) {
  iLifetime=iTime;			// Zeit bis das Ding gelöscht wird
  iX=iXPos;				// Von der Mitte abweichende Position (X)
  iY=iYPos;				// Von der Mitte abweichende Position (Y)
  iSpeed = FBRT_BaseSpeed + iExtraSpeed;// Geschwindigkeit: Basisgeschwindigkeit + Zusatz
  SetR((iAngle+90)*(GetDir(pTrg)*2-1));	// Winkel, 0 wäre 90° für Dir=1 bzw 270° für Dir=0
  SetRDir(iAngleDir);			// Rotationsgeschwindigkeit
  SetAction("Exist",pTrg);		// Objekt an dass es pseudo-attached wird
  fLethal = fDeadly;
}

func AttachToVertex(int iVertex)
{
  fVertexAttach = true;
  iAttachVertex = iVertex;
}

func Timer() { // alle 4 Frames
  var iXDir,iYDir,iAngle;
  var pActTarget = GetActionTarget();
  // Kein Gas-Spucker: löschen
  if(!pActTarget) return(RemoveObject());

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
    CreateParticle("Smoke",iX2,iY2,iXDir,iYDir,RandomX(50,100),RGB(255,255,255));	 

    // Gas-Objekte in die Richtung verschießen
    CreateObject(_GSH,
		 iX2+Sin(GetR()+iAngle,10),
		 iY2-Cos(GetR()+iAngle,18),
		 GetOwner())->ActivateGas(iXDir, iYDir, fLethal);
  }
}

// Globale Funktion zum Erstellen
global func CreateGasShoot(object pCaster, int iTime, int iX, int iY, int iRot, int iRotSpeed, int iExtraSpeed)
  {
  return(CreateObject(_GSH,0,0,GetOwner(pCaster))->_GSH::Activate(pCaster,iTime, iX, iY, iRot, iRotSpeed, iExtraSpeed));
  }

/* -- Gasobjekte -- */

public func ActivateGas(int iXDir, int iYDir, bool fDeadly)
  {
  // Gas-Aktivität
  SetAction("GasObj");
  SetCategory(2);
  SetXDir(iXDir); SetYDir(iYDir);
  fLethal=fDeadly;
  return(1);
  }

protected func GasTimer()
  {
  DoCon(+20);
  // Gas erzeugen
  CreateObject(GAS_);
  }

protected func GasRemoval() { Smoke(0, 0, 24); return(RemoveObject()); }