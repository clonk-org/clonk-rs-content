#strict

#include QGLG

local counter;

func ExplodeDelay() { return(20+counter); }

func SetCounter(int time)
{
	counter = time*3;
}

func Launch(int xdir, int ydir, int iDmg)
{
	_inherited(xdir, ydir, iDmg);
	RemoveEffect("HitCheck",this());
}

func HitObject()
{
  RemoveEffect("Grenade", this(),0,1);
//  CastObjects(BRNH,Random(3)+6,30);
  BlastObjects(GetX(), GetY(), iDamage/10);
  RemoveObject();
  var l = iDamage/20;
  var u = iDamage/10;
  for(var i; i < 60; i++)
  {
  	var r = (360/60)*i;
  	var x,y;
  	x = Sin(r,5);
  	y =-Cos(r,5);
  	if(GBackSolid(x,y))
  		continue;
  	var nail = CreateObject(QCNL,x,y,GetController());
  	//int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize
  	if(nail)
  		nail->Launch(r,150+Random(50), 500, 3, 160, RandomX(l,u));
  }
  
  Sound("GLExplode");
}
