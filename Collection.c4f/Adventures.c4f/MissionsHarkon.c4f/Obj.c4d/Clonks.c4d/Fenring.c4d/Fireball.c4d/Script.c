/*--- Flint ---*/

#strict 2

public func ExplodeSize() 
{ 
  return(18); 
}
local HitCount;
protected func Hit()
{
  if(HitCount++ > 4) return RemoveObject();
	pLastClonk = 0;
	var iDir = GetDir();
	if(GBackSolid( 4)) iDir = 1;
	if(GBackSolid(-4)) iDir = 0;
  if(iDir)
	{
		SetDir(0);
		SetYDir(0);
		SetXDir(-60-Random(10));
	}
	else
	{
		SetDir(1);
		SetYDir(0);
		SetXDir(60+Random(10));
//		SetComDir(COMD_Right);
	}
}

/* Fliegt */

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

func Initialize()
{
	SetAction("Travel");
	AddEffect("Meteorsparkle",this,100,3,this);
	HitCount = -1;
	Hit();
}

func FxMeteorSparkleStart(obj, effect, iTemp)
{
	if(iTemp) return;
}

func TestX()
{
  if(Inside(GetX()-506, -5, 5)) return 1;
  if(Inside(GetX()-605, -5, 5)) return 1;
  if(Inside(GetX()-697, -5, 5)) return 1;
  return 0;
}

func FxMeteorsparkleTimer(obj, effect, time)
{
	iYOffset = Sin(time*7, 10);
	var x=0, y=iYOffset;//obj->GetX(), y=obj->GetY();
	//if(!ObjectCount(_TFN) && TestX()) CreateObject(_TFN, 1, -5);
	CreateParticle("FireballSmoke",x,y,Sin(Random(360),2),Cos(Random(360),2),RandomX(120,180)*2,RGBa(100,100,100,70));
	for(var i=0; i<6; i++) CreateParticle("MagicFire",x,y,Sin(Random(360),RandomX(5,6)),Cos(Random(360),RandomX(5,6)),RandomX(50,90)*2,HSL(Random(50), 200+Random(25), Random(100)));
	CreateParticle("MagicSpark",x,y,Sin(Random(360),RandomX(15,33)),Cos(Random(360),RandomX(15,33)),RandomX(30,70)*2,RGB(255,255,255));
	if(GetXDir()== 0) Hit();
}
