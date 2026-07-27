/*-- Meteor --*/

#strict

local explosion_base;

func Initialize()
{
	Sound("Strike");
}

protected func Entrance()
{
	SetAction("Idle");
}

protected func SmokeTrail()
{
  /*Smoke(0, 0, 5);
  Smoke(0, -5, Random(7));
  var i = Random(50);
  while (i--) {
    var c = 20+Random(41);
    CreateParticle("PxSpark", Random(11)-5, Random(11)-5, 0, 0, c,
      RGBa(255, Random(100), c*4, 128));
  }
  CreateParticle("PxSpark", 0, 0, 0, 0, 20,
    RGBa(255, 155, 0, 128));*/
    
   var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-30,30);
    var xdir = +Sin(GetR()+rand,20);
    var ydir = -Cos(GetR()+rand,20);

    CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,0),0,0);
  }

}

public func Completion() {
  explosion_base = 8;
  SetAction("Evaporate");
}

public func SetExplosionBase(new_base) {
  explosion_base = new_base;
}

protected func Hit() {
  Explode(explosion_base+Random(10));
}
