/*-- Knochen sollen ausfaden --*/

#strict 2
#appendto BONE
#appendto ROCK
#appendto WOOD
#appendto SPER
#appendto SWOR
#appendto AXE1
#appendto IDLS


func Initialize() {
  AddEffect("FadeOut",this,20,10,this);
  if(GetID()==BONE) SetObjectLayer(this);
  return _inherited();
}

protected func FxFadeOutStart(pTarget,iEffectNumber)
{
  return 1;
}

protected func FxFadeOutTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(Contained(pTarget))
  {
    EffectVar(0,pTarget,iEffectNumber) = 0;
    return 1; 
  }
  EffectVar(0,pTarget,iEffectNumber) = BoundBy(EffectVar(0,pTarget,iEffectNumber)+2,0,152);
 
  if(EffectVar(0,pTarget,iEffectNumber) >= 150)RemoveObject(pTarget);
 
  SetClrModulation(RGBa(255,255,255,EffectVar(0,pTarget,iEffectNumber)),pTarget);

  return 1;
}
