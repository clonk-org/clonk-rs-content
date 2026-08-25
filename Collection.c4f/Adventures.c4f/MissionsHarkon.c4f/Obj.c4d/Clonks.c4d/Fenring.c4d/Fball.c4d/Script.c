/*-- Feuerklumpen --*/

#strict

//#2do: sounds finden

local iBounces,iCAngle, fExplodeOnHit, iTotalBounces, iLastBouncePos;


  /* Object-Calls */

protected func Initialize() // Initialisierung
  {
  Departure();
  SetMaxBounces(RandomX(8,12));
  iCAngle=Random(360);
  }
  
public func SetExplodeOnHit()
  {
  fExplodeOnHit = true;
  }

protected func Departure()  // Austritt
  {
//  Incinerate();
  SetAction("Bounce");
  SetRDir(RandomX(-50,50));
  }

protected func Entrance()   // Eintritt
  {
  SetAction("Exist");
  }

protected func Hit()        // Auftreffen
  {
  //Sound
  Sound("LumpHit*");
  // Explosion? Nur nicht an SolidMask
/*  if (fExplodeOnHit)
    {
    var iBouncePos = GetX() + (GetY()+10)*LandscapeWidth();
    if (iBouncePos != iLastBouncePos)
      Explode(10, CreateObject(COAL,0,0,GetOwner()));
    iLastBouncePos = iBouncePos;
    }*/
  //irgendwann ist er alle
//  if(--iBounces < 0 || ++iTotalBounces >= 80) return(ChangeDef(COAL));

   //einmal auftanken bitte! (in lava)
  if(GetMaterialVal("Incindiary","Material",GetMaterial(0,0)))
     iBounces+=5;

  //Geschwindigkeit neu bestimmen (völlig unkontrolliert herumspringen)
  SetBounceSpeed(RandomX(10,40),RandomX(25,45));
  //drehen
  SetRDir(RandomX(-50,50));
  }
  
local iOldX, iOldY, pLastClonk, iYOffset;

func Travel() {
  // Treffer-Überprüfung
  CheckHit();
  iOldX = GetX(); iOldY = GetY()+iYOffset;
}

private func CheckHit()
{
  // Nur wenn schon passende Werte da sind
  if(!iOldX && !iOldY) return;
  // Distance zur letzten Position berechnen
  var iXDist = GetX()-iOldX;
  var iYDist = GetY()+iYOffset-iOldY;
  // Treffer-Überprüfung
  var iSteps = Max(Abs(iXDist/4), 1);
  if(Abs(GetXDir())<Abs(GetYDir())) iSteps = Abs(iYDist/4);
  var iX, iY, pObj;
  // Mit Schleife alle Zwischenpunkte abklappern
  for(var i = iSteps; i; i--)
  {
    iX = -iXDist*i/iSteps; iY = -iYDist*i/iSteps;
    if(!iY) iY = 1;
//    CreateParticle("NoGravSpark", iX, iY+iYOffset, 0, 0, 30, RGB(i*255/iSteps, (iSteps-i)*255/iSteps));
    if(pObj = FindObject(0, iX, iY+iYOffset, 0, 0, OCF_Alive, 0, 0, NoContainer())) return HitLiving(pObj);
  }
}

/* Treffer */

func HitLiving(pTarget) {
  if(!Hostile(GetOwner(), GetOwner(pTarget))) return;
	if(pTarget == pLastClonk) return;
	Sound("Blast2");
  Fling(pTarget, -2+5*GetDir(), -3);
  for(var i=0;i<5;++i)
    CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,5),0,0,RandomX(80,200),RGB(255,40,0));
  DoEnergy(-4,pTarget);
	Hit();
	pLastClonk = pTarget;
}



  /* Timer */

protected func Bouncing()       // StartCall in Aktion "Bounce"
  {
  //In Wasser löschen
//  if(!OnFire()) ChangeDef(COAL);
CreateParticle("MagicFire",0,0,Sin(Random(360),RandomX(5,6)),Cos(Random(360),RandomX(5,6)),RandomX(50,90)*2,HSL(Random(50), 200+Random(25), Random(100)));
Travel();
  }

protected func Existing()
  {
  var iR,iG;
  iCAngle+=10;
  iR = Sin(iCAngle,24)+24;
  iG = Cos(iCAngle,32)+32;
//  SetClrModulation(RGB(iR,iG,64));
  }


  /* Script-Aufrufe */

private func SetBounceSpeed(iXDir,iYDir)    // Abprall
  {
  // normalerweise nach oben springen, doch an der Decke prallt er nach unten ab
  if(GBackSolid(0,-7))      SetYDir(2);
  else                      SetYDir(-iYDir);

  // falls er an eine Wand kommt, abprallen. Sonst chaotisch rumspringen
  if(GBackSolid(+7,0))      SetXDir(-iXDir);
  else if(GBackSolid(-7,0)) SetXDir(+iXDir);
  else                      SetXDir((Random(2)*2-1)*iXDir);
  }

public func SetMaxBounces(iNewBounces)      // Lebenszeit setzen
  {
  iBounces=iNewBounces;
  }
