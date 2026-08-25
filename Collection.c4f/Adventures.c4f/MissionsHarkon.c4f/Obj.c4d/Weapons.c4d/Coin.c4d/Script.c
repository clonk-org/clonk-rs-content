/*-- Münze --*/

#strict 2

/* Aufschlag */

local fActive;
local iCounter;

public func AddCounter() { iCounter++; }
public func GetCounter() { return iCounter; }

protected func Hit()
{
  //AI_AlarmCurious(this, 150);
  Sound("CrystalHit*");
  Sparkle();
  fActive = 1;
  SetAction("Idle");
  return 1;
}

func Initialize()
{
  SetAction("Fly");
}

func RejectEntrance()
{
  if(fActive)
    return 1;
}

func PickUp(pContainer)
{
	fActive = 0;
	Enter(pContainer);
}

func Timer()
{
  if(Random(3) && !Contained()) Sparkle();
}

func Sparkle()
{
  CreateParticle("MaterialSpark", 0,0, 0,0, 5*5+Random(5*5), RGBa(255,255,128+Random(128),0));
}