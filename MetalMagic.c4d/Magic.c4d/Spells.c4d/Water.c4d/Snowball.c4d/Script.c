/* Schneeball */

#strict 2

local iOldX, iOldY;

func Activate(pCaster,pRealcaster) { 
  // Den Clonk ermitteln
  if(pRealcaster) pCaster=pRealcaster;  

  // Nicht im Gebäude zaubern
  if(Contained(pCaster))
  {
    Sound("Error");
    PlayerMessage(GetOwner(pCaster), "$must_be_in_free$", pCaster);
    RemoveObject();
    return 0;
  }

  SetAction("Wait");
  AddEffect("IceSnowballNSpell", this, 110, 0, this);   
  
  // Zielen wenn möglich
  if (pCaster->~DoSpellAim(this)) return 1;
	
  Sound("Electric"); 
  SetAction("Travel");
  SetPosition(GetX()+(GetDir(pCaster)*2-1)*20,GetY());
  SetXDir((GetDir(pCaster)*2-1)*100);
  SetCategory(4);
  iOldX = GetX(); iOldY = GetY();
  return 1;
}

func FxIceSnowballNSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
  if(!fTemp)  RemoveObject(pTarget);
  return 1;
}

// Zwischenaufruf: Zielrichtung geändert
public func AimingAngle(object pCaller, int iAimAngle)
  {
  SetPosition(GetX(pCaller)+Sin(iAimAngle, 20),GetY(pCaller)-Cos(iAimAngle, 20));
  return 1;
  }

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAimAngle)
{
  Sound("Electric"); 
  SetAction("Travel");
  SetPosition(GetX(pCaller)+Sin(iAimAngle, 20),GetY(pCaller)-Cos(iAimAngle, 20));
  SetXDir(Sin(iAimAngle, 100));
  SetYDir(-Cos(iAimAngle, 100));
  SetCategory(4);
  iOldX = GetX(); iOldY = GetY();
  return 1;
}
  
/* Fliegt */

func Travel() {
  var pLiving;
//  CastPXS("Snow",RandomX(8,12),RandomX(20,30));
  CastParticles ("Snow", RandomX(8,12), RandomX(20,30), 0, 0, 10, 50, RGB(255,255,255), RGB(255,255,255));
  if(GetXDir()==0 && GetYDir()==0) Hit();
  // Treffer-Überprüfung
  CheckHit();
  iOldX = GetX(); iOldY = GetY();
//  if(pLiving=FindObject(0,-6,-6,12,12,OCF_Alive,0,0,NoContainer())) return HitLiving(pLiving);
}

private func CheckHit()
{
  // Nur wenn schon passende Werte da sind
  if(!iOldX && !iOldY) return;
  // Distance zur letzten Position berechnen
  var iXDist = GetX()-iOldX;
  var iYDist = GetY()-iOldY;
  // Treffer-Überprüfung
  var iSteps = Abs(iXDist/4);
  if(Abs(GetXDir())<Abs(GetYDir())) iSteps = Abs(iYDist/4);
  var iX, iY, pObj;
  // Mit Schleife alle Zwischenpunkte abklappern
  for(var i = iSteps; i; i--)
  {
    iX = -iXDist*i/iSteps; iY = -iYDist*i/iSteps;
    if(!iY) iY = 1;
//    CreateParticle("NoGravSpark", iX, iY, 0, 0, 30, RGB(i*255/iSteps, (iSteps-i)*255/iSteps));
    if(pObj = FindObject(0, iX, iY, 0, 0, OCF_Alive, 0, 0, NoContainer())) return HitLiving(pObj);
  }
}

/* Treffer */

func Hit() {
  CastParticles ("Snow", RandomX(8,12), RandomX(20,30), 0, 0, 10, 50, RGB(255,255,255), RGB(255,255,255));
//  if(pTarget) HitLiving(pTarget);
  RemoveObject();
}
    
func HitLiving(pTarget) {
  DoEnergy(-8,pTarget);
  for(var i=0;i<5;++i)
    CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,5),0,0,RandomX(80,200),RGB(255,40,0));
  // Ist pTarget ein Eskimo, dann wird ihm auch noch Wärme entzogen
  pTarget->~MagicWarm(-650);
  Freeze(pTarget);
  Hit();
}

public func IsAimerSpell() { return 1; }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMWT; }
public func GetSpellStaffCombo(pMage) { return ("43"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
