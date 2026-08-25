/* Rakete */

#strict
#include MISS

public func Acceleration() { return(2); }
public func MaxTime() { return(70); }
public func MaxSpeed() { return(200); }


public func Launch(int iAngle, int iDmg, int iSpd)
{
	if (!iSpd) iSpd=55;
  iSpeed = iSpd; //Speeed!
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");

  AddLight(100,RGB(255,127,0),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter);
  Sound("rocketfly",0,this(),0,0,1);
}

private func HitObject(object pObject) {
  exploding = true;
  
  Sound("rocketHit");
  
  DoDmg(iDamage*2/3, DMG_Explosion, pObject);
  
  //BlastObjects(GetX(), GetY(), iDamage/3);
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  Explode(iDamage/2,0,0,0,true,true);
}

private func Smoking() {

  //var dist = Distance(0,0,GetXDir(),GetYDir());
  //var maxx = +Sin(GetR(),dist/10);
  //var maxy = -Cos(GetR(),dist/10);
  //var ptrdist = 100;

	var x = +Sin(GetR(),-4);
	var y = -Cos(GetR(),-4);

  //var x = -maxx*i/dist + xoff;
  //var y = -maxy*i/dist + yoff;


  var rand = RandomX(-30,30);
  var xdir = +Sin(GetR()+rand,20);
  var ydir = -Cos(GetR()+rand,20);

  CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(60,80),RGBa(255,200,200,60),0,0);
  CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,0),0,0);
}

// resistenz!
func OnDmg(int dmg, int type)
{
	if(type == DMG_Energy)
		return(40);
	return(20);
}
