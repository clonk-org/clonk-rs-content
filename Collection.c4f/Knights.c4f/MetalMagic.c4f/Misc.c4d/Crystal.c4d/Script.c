/*--- Kristall ---*/

#strict 2

local pTarget;
local idSpell;
local fAdventureCrystal; // Adventure-Kristallverhalten

public func Initialize()
{
  SetAction("Ready");
}

public func DoSpellAim(pSpell)
{
//  Message("ActivateAngle(%s, %d);",this, GetName(this), GetR());
  pSpell->~ActivateAngle(this, GetR());
  SetAction("Magic");
  return 1;
}

protected func Timer()
{
  for(var pObj in FindObjects(Find_OCF(OCF_CrewMember), Find_Distance(30), Find_NoContainer()))
  {
    if (!fAdventureCrystal)
    {
      if(GetDir() != GetDir(pObj)) continue;
      if(GetAction(pObj)!="Walk" && GetAction(pObj) != "Jump") continue;
    }
    var angle = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj));
    if(!GetDir()) angle = 360-angle;
    if(Abs(Abs(angle)-GetR())>40) continue;
    pTarget = pObj;
    var id = idSpell;
    if(!id) id = _BLA;
    CreateObject(id)->Activate(this);
  }
  if(!Mod(GetActTime(), 20))
  {
    CastParticles("NoGravSpark", 1, 4, 0, 0, 40, 50, RGB(100,100,200), RGB(200,100,200), this);
    CreateParticle("NoGravSpark", 0, 0, Sin(Abs(GetR()), 8)*(-1+2*GetDir()),-Cos(Abs(GetR()),8), 40, RGB(255), this);
  }
}

public func SetAdventure() { fAdventureCrystal = true; }
