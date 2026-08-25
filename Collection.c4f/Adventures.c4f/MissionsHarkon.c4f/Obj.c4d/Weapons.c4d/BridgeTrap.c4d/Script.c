/*-- Brückensegment --*/

#strict 2

local is_locked;	// Ist im Boden verankert?

protected func Initialize() {
  // Im Freien erzeugte Brückensegmente sind verankert
//  if (!Contained())
//    Lock(1, 1);
  return(1);
}

protected func Entrance() {
  // Im Behälter: lösen
  Release(1);
  return(1);
}

public func ControlDigDouble(caller) {
  [$TxtLock$]
  // Wenn verankert lösen
  if (is_locked) Release();
  // sonst verankern
  else Lock();
  return(1);
}

private func Lock(quiet, dont_descend) {
  SetAction("Locked");
  SetSolidMask(0, 11, 40, 3);
  is_locked = 1;

  if (!quiet)
    Sound("Connect");

  // Verankern
  if (!dont_descend)
    if (!GBackSolid(0, 10))
      SetPosition(GetX(), GetY()+10);
  return(1);
}

private func Release(quiet, dont_ascend) {
  SetAction("Idle");
  SetSolidMask();
  is_locked = 0;

  if (!quiet)
    Sound("Connect");

  // Aus der Erde lösen
  if (!dont_ascend) {
    var ascend_pixels = 0;
    while (Stuck() && (++ascend_pixels < 12))
      SetPosition(GetX(), GetY()-1);
  }

  return(1);
}

func TestEnemy()
{
  var found = 0;
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_OCF(OCF_CrewMember), Find_InRect(-15,-30,30,40)))
  {
    if ( (GetContact(obj, -1) & CNAT_Bottom) && !(obj->GBackSolid(-5, +35)) && !(obj->GBackSolid(+5, +35)) && !GetEffect("DoTumble", obj))
    {
      found = 1;
      DoTrap(obj);
    }
  }
  if(found)
  { 
    SetAction("BreakDown");
    SetSolidMask();
  }
}

func TestEnemy2()
{
  var found = 0;
  for(var obj in FindObjects(Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_OCF(OCF_CrewMember), Find_InRect(-15,-30,30,40)))
  {
    if ( (obj->GetY() < GetY()) && !(obj->GBackSolid(-5, +25)) && !(obj->GBackSolid(+5, +25)) && !GetEffect("DoTumble", obj) )
    {
      found = 1;
      DoTrap(obj);
    }
  }
  SetClrModulation(RGBa(255,255,255, GetActTime()*255/35));
  if(GetActTime() > 35) RemoveObject();
}

func DoTrap(obj)
{
      var i = ContentsCount(0, obj);
      while(i) obj->Exit(obj->Contents(i--,0,-10));
      obj->SetAction("Tumble");
      obj->SetXDir(0);
      obj->SetYDir(-10);
      AddEffect("DoTumble", obj, 1, 1, 0, GetID()); 
}

func FxDoTumbleTimer(obj, number, time)
{
  if(time > 35*6) return -1;
  if(obj->GetAction() == "Jump")
  {
    var i = ContentsCount(0, obj);
    while(i) obj->Exit(obj->Contents(i--,0,-10));
    obj->SetAction("Tumble");
  }
  else if(GetContact(obj, -1) & CNAT_Bottom)
    obj->SetAction("FlatUp");
  
  if(obj->GetAction() == "Tumble")
  {
    var i = ContentsCount(0, obj);
    while(i) obj->Exit(obj->Contents(i--,0,-10));
  }
}
/*
global func RemoveObject(obj)
{
  if(!obj) obj = this;
  if(GetID(obj) == ZAPN) MakeError();
  _inherited(obj, ...);
}

global func MakeError() { a }
*/
public func IsBridge() { return(1); }

public func IsLocked() { return(is_locked); }

public func IsSawProduct() { return 1; }

// Nicht an die Dampflok koppeln lassen
public func NoPull()
{
  return IsLocked();
}
