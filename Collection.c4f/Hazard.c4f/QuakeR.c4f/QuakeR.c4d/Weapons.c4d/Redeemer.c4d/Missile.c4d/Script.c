/* Rakete */

#strict
#include MISS

local fControlled;
local iTurn;
local iDamage2;

public func Acceleration() { return(2); }
public func MaxTime() { return(100); }
public func MaxSpeed() { return(40); }
public func MaxTurn() { return(10); }
public func TurnStep() { return (2); }

//-> RGB(200,50,255)
// = 13120255
//-> RGB(50,200,255)
// = 3328255
static const REDEEMER_color = 3328255;

public func GetSize() { return(iDamage); }
public func GetSize2() { return(iDamage2); }

public func Launch(int iAngle, int iDmg, int iDmg2, int iSpd, bool fCtd)
{
	if (!iSpd) iSpd=55;
  iSpeed = iSpd; //Speeed!
  iDamage = iDmg;
  iDamage2 = iDmg2;
  fControlled = fCtd;
  if(!iDamage) iDamage = 70;
  if(!iDamage2) iDamage2 = 200;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");
//RGB(200,127,255)
  AddLight(100,RGB(127,200,255),this(),GLOW);

	Sound("RDFly",0,this(),0,0,1);

  AddEffect("HitCheck", this(), 1,1, 0, SHT1,shooter);
  if (fControlled)
  {
    SetCursor(GetOwner(shooter),this());
    SetPlrViewRange(200, this());	
  }
}

private func HitObject(object pObject) {
  exploding = true;
  AddEffect("RedeemerExplosion",0,2,1,0,GetID(),GetX(),GetY(), GetController(),this());

	Sound("RDhit");

  RemoveObject();
}

private func Accelerate() { 
 //Gehört hier nicht hin, egal!
	if(fControlled) SetR(GetR()+iTurn);
  if(iSpeed < MaxSpeed())
    iSpeed += Acceleration();

  SetXDir(+Sin(GetR(),iSpeed));
  SetYDir(-Cos(GetR(),iSpeed));
}

public func ControlLeft()
{
  // Bei Klassisch für jedes Mal drücken etwas schneller drehen
  if(!GetPlrCoreJumpAndRunControl(GetController()))
  {
    if(iTurn>0)
      iTurn = 0;
    else iTurn=BoundBy(iTurn-TurnStep(),-MaxTurn(),MaxTurn());
  }
  // JnR gleich volle Drehungsgeschwindigkeit
  else iTurn-=MaxTurn();
}

public func ControlRight()
{
  // Bei Klassisch für jedes Mal drücken etwas schneller drehen
  if(!GetPlrCoreJumpAndRunControl(GetController()))
  {
    if(iTurn<0)
      iTurn = 0;
    else iTurn=BoundBy(iTurn+TurnStep(),-MaxTurn(),MaxTurn());
  }
  // JnR gleich volle Drehungsgeschwindigkeit
  else iTurn+=5;
}

// Die restlichen Tasten mit Stop belegen, damit Klassisch auch gerade fliegen kann
public func ControlDig()
{
  iTurn = 0;
}

public func ControlUp()
{
  iTurn = 0;
}

public func ControlDown()
{
  iTurn = 0;
}

// Bei JnR wenn die Taste losgelassen wird auch keine Drehung mehr
public func ControlLeftReleased()
{
  iTurn = 0;
}

public func ControlRightReleased()
{
  iTurn = 0;
}

// Werfen löst den Eisschlag^W^W die Rakete natürlich..! aus
public func ControlThrow() { Hit(); }


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

  CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(60,80),RGBa(50,200,255,60),0,0);
  CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),REDEEMER_color,0,0);
}

global func FxRedeemerExplosionStart(object targ, int nr, int tmp, int x, int y, int owner, object callback)
{
	if(tmp)
		return(1);
	
	EffectVar(0,targ,nr) = x;
	EffectVar(1,targ,nr) = y;
	EffectVar(2,targ,nr) = owner;
	if(callback)
	{
		EffectVar(3,targ,nr) = callback->GetSize();
		EffectVar(4,targ,nr) = callback->GetSize2();
	}
	else
	{
		EffectVar(3,targ,nr) = 70;
		EffectVar(4,targ,nr) = 200;
	}
} 

global func FxRedeemerExplosionTimer(object targ, int nr, int time)
{
	var r = 20 + EffectVar(4,targ,nr)/2;
	
	var xoff,yoff;
	var owner;
	var x,y;
	var count = 20;
	var step = 360/count;
	var size = EffectVar(3,targ,nr);
	var size2 = EffectVar(4,targ,nr);
	
	xoff = EffectVar(0,targ,nr);
	yoff = EffectVar(1,targ,nr);
	owner = EffectVar(2,targ,nr);
	
	// zuerst Ansammeln...!
	if(time <= 10)
	{
		for(var i; i < count; i++)
		{
			x = Sin(time*3/2 + i*step, Sin(100+time*8,r));
			y = Cos(time*3/2 + i*step, Sin(100+time*8,r));
			CreateParticle("RedeemerBlast",xoff+x,yoff+y,0,0,size*2);
			for(var obj in FindObjects(Find_NoContainer(),Find_Distance(size/5,xoff+x,yoff+y)))
			{
				if(GetOCF(obj) & OCF_CrewMember) Fling(obj, (xoff - obj->GetX())/5, (yoff - obj->GetY())/5);
				obj->DoDmg(50, DMG_Explosion, obj, 0, owner+1);
			}
		}
	}
	// dann ein bisschen in dem Bereich rumsprengen!
	else if(time < 100)
	{
		var angle = Random(360);
		x = Sin(angle, Random(r));
		y = Cos(angle, Random(r));
		CreateParticle("RedeemerBlast",xoff+x,yoff+y,0,0,size2*2);
		CreateParticle("EMPShock",xoff+x,yoff+y,0,0,size2*2*2, REDEEMER_color);
		for(var obj in FindObjects(Find_NoContainer(),Find_Distance(size2/5,xoff+x,yoff+y)))
		{
			obj->DoDmg(150, DMG_Explosion, obj, 0, owner+1);
		}
	}
	// dann das Finale!
	else
	{
		// Kabumms!
		CreateParticle("RedeemerBlast",xoff+x,yoff+y,0,0,r*10);
		CreateParticle("EMPShock",xoff+x,yoff+y,0,0,r*10*2, REDEEMER_color);
		for(var obj in FindObjects(Find_NoContainer(), Find_Distance(r,xoff+x,yoff+y)))
		{
			obj->DoDmg(500, DMG_Explosion, obj, 0, owner+1); // der sichere Tod!
		}
		return(-1);
	}
}
