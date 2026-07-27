/*-- Neues Objekt --*/

#strict 2
local ID,target;
func InitializeToObject(obj) {
 ID=GetID(obj);
 target=obj;
 SetAction("Be",target);
  return 1;
}

protected func CheckTarget()
{
//erstmal prüfen, ob kein Target mehr da oder so
if(!target
|| GetID(target)!=ID
|| OnFire(target))return RemoveObject();

//Jetz Position.
var x;
var y;
var r=GetR(target);
var targetheigth=35;
x=GetX(target)+Cos(GetR(target)-90,targetheigth);
y=GetY(target)+Sin(GetR(target)-90,targetheigth);

SetPosition(x,y,this);
SetR(r,this);

//Geh00im entweder var auf null setzen oder sontwas tolles machen
var aimingstuff=0;
if(aimingstuff)
{
var x=GetX(target)+Sin(GetR(), 13);
var y=GetY(target)-Cos(GetR(), 13);
var xdir=Sin(GetR(), target->CannonPower(this))*10;
var ydir=-Cos(GetR(), target->CannonPower(this))*10;
SimFlight (x, y, xdir,ydir); 
//var size=BoundBy(Distance(GetX(target),GetY(target),x-GetX(target),y-GetY(target)),5,1000)/10;

CastParticles("NoGravSpark",10,3,x-GetX(),y-GetY(),30,40,RGB(0,0,0),RGB(255,255,255));
}
}
