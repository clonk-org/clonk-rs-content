/*-- Fire --*/

#strict 2
static const FRST_Speed=60;

local timer;
local iOldX, iOldY;

func Initialize() {
  timer=0;
  return 1;
}

public func Activate(pCaster,iAngle)
{
 SetAction("Fly");
 var xDir,yDir;
 xDir=Cos(iAngle-90,FRST_Speed);
 yDir=Sin(iAngle-90,FRST_Speed);
 SetXDir(xDir+RandomX(-10,10));
 SetYDir(yDir+RandomX(-10,10));
 iOldX = GetX(); iOldY = GetY();
}

protected func Hit()
{
  if(CheckHit()) return;
  RemoveObject();
}

protected func Check()
{
  if(++timer > 100)RemoveObject();

  if(Random(2))CreateParticle("Fire",RandomX(-4,4),RandomX(-4,4),GetXDir()/2,GetYDir()/2,70,RGB(255,255,255));
  else CreateParticle("Fire2",RandomX(-4,4),RandomX(-4,4),GetXDir()/2,GetYDir()/2,70,RGB(255,255,255));
  CreateParticle("PSpark",0,0,GetXDir()/4,GetYDir()/4,50,RGB(255,20,20));
 
  // Auf Treffer prüfen
  CheckHit();
  iOldX = GetX(); iOldY = GetY();
/*  var pClonk=0;
  while(pClonk=FindObject(0,0,-1,0,0,OCF_Alive,0,0,NoContainer(),pClonk))
  {
   if(!ObjectCount(NTMG))if(Hostile(GetOwner(),GetOwner(pClonk)))continue;
   Punch(pClonk,4);
   RemoveObject();
  }*/
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

private func HitLiving (living)
{
  Sound("Inflame", living);
  // Schaden machen
  DoEnergy(-(2+Random(4)),living);
  return RemoveObject();
}

protected func Destruction()
{
 CastParticles("FSpark",6,5,0,0,50,100,RGB(200,10,10),RGB(255,100,20));
}
