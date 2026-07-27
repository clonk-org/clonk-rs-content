/*-- Bolzen --*/

#strict 2

#include ARRW

/* Paketeigenschaften */
public func PackTo() { return BOTP; }
public func IsBolt() { return 1; }
public func IsArrow() { return 0; }

public func BoltStrength() { return 30+Random(6); }

public func IsDangerousToAirTravel() { return 1; }

local iOldX, iOldY;

/* Wird abgefeuert */
public func Launch()
{
  iOldX=GetX();
  iOldY=GetY();
  SetAction("Shot");
  SetCategory(4);
}

public func RejectEntrance(a, b)
{
  if(GetAction()=="Shot") return 1;
  return _inherited(a,b);
}

/* Aufschlag */
protected func Hit()
{
  if(GetAction()=="Shot")
  {
    var target = FindTarget();
    if(target)
      return HitTarget(target);
    SetCategory(1);
  }
  SetAction("Idle");
}

/* Ziel prüfen */
private func InFlight()
{
  SetR(Angle(0, 0, GetXDir(), GetYDir()));
  var target = FindTarget();
  if(target)
    return HitTarget(target);
}

protected func FindTarget()
{
  var obj;
 
  var x = GetX()-iOldX;
  var y = GetY()-iOldY;

  var obj;
  var length = Abs(x/4);
  if(Abs(GetXDir()) < Abs(GetYDir())) length = Abs(y/4);

  var cX,cY;
  for(var cnt=length;cnt;cnt--)
  {
    cX=-x*cnt/length;
    cY=-y*cnt/length;
//    CreateParticle("NoGravSpark", cX, cY, 0, 0, 20, RGB(255));
    if(obj=FindObject2(Find_AtPoint(cX,cY),Find_NoContainer(),Find_Layer(GetObjectLayer()),Find_OCF(OCF_Alive, OCF_Living)))
      return obj;
  
    // Osterei!
    var flnt;
    if(flnt = FindObject2(Find_AtPoint(cX,cY),Find_Layer(GetObjectLayer()),Find_NoContainer()))
      if(flnt->~ExplodeSize())
        if(ActIdle(flnt))
        { flnt->Hit(); Hit(); }
  }
 
  iOldX=GetX();
  iOldY=GetY();
 
  return 0;
}

/* Treffer */

private func HitTarget(pObject)
{
  Sound("ArrowHit");
  var iDamage = BoltStrength();
  // Im oberen Drittel getroffen?
  if(GetY()<GetY(pObject)+GetDefHeight(GetID(pObject))/3+GetDefOffset(GetID(pObject), 1) && !pObject->~HasArmor())
  {
    // Headshot!!
    iDamage = GetPhysical("Energy", 0, pObject)*8/10000;
    Sound("Punch3");
    Message("!", pObject);
  }
  iDamage = iDamage * (100 - pObject->~IsShielded()) / 100;
  DoEnergy(-iDamage,pObject);
  if(Random(3)) Fling(pObject,GetXDir()/80,GetYDir()/80-2);
  RemoveObject();
}
