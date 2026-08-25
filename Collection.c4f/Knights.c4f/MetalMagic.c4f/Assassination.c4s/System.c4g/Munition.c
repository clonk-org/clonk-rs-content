/*-- Neues Script --*/

#strict 2

#appendto BLTP

func Initialize() {
  AddEffect("FadeOut",this,20,10,this);
  return _inherited();
}


protected func FxFadeOutStart(pTarget,iEffectNumber)
{
 EffectVar(0,pTarget,iEffectNumber)=0;
 return 1;
}

protected func FxFadeOutTimer(pTarget,iEffectNumber,iEffectTime)
{
 EffectVar(0,pTarget,iEffectNumber)=BoundBy(EffectVar(0,pTarget,iEffectNumber)+2,0,152);
 
 if(Contained(pTarget))EffectVar(0,pTarget,iEffectNumber)=0;
 
 if(EffectVar(0,pTarget,iEffectNumber)>=150)RemoveObject(pTarget);
 
 SetClrModulation(RGBa(255,255,255,EffectVar(0,pTarget,iEffectNumber)),pTarget);

 return 1;
}
