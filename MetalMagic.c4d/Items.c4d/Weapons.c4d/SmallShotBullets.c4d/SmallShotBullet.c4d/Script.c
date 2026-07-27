/*-- Pfeil --*/

#strict 2

#include BLLT

/* Pfeileigenschaften */
public func PackTo() { return SBLP; }
public func IsBullet() { return 1; }
public func BulletStrength(){return 1+Random(2);}


public func Launch(quiet,bar,nosplit)
{
 if(!nosplit && !quiet)
  for(var cnt=0;cnt<18;cnt++)
  {
  var blt=CreateObject(GetID(),0,0,GetOwner());
  blt->LocalN("pLauncher")=pLauncher;
  blt->Launch(1,0,1);
  SetXDir(GetXDir()+RandomX(-37,37),blt);
  SetYDir(GetYDir()+RandomX(-37,37),blt);
  
  }
  
  return _inherited(quiet,bar,nosplit);
} 

protected func InFlight()
{
 if(GetActTime()>17 && GetAction() == "Shot")Stop();
 return _inherited();
}

protected func Tail()
{ 
   var alpha=0;
   if(GetAction() == "Down")alpha=10;
   CreateParticle("BulletTail",0,0,GetXDir(),GetYDir(),BoundBy(GetActTime(),0,15)*10,RGBa(255,255,255,10+alpha),this);
}

/* Treffer */

private func HitTarget(pObject)
{
  Sound("BulletHit*");

  //Punch(pObject,BulletStrength());
  //Punch simulieren, weil doof 
  DoEnergy(-BulletStrength(),pObject);
  if(!Random(3))Fling(pObject,GetXDir()/80,GetYDir()/80-2);
  
  RemoveObject();
}


