/* Wipftrank */

#strict

protected func Hit() { Sound("Crystal*"); }

public func IsPotion() { return(1); }
public func BrewTime() { return(6*35); }

public func EffectDuration() { return(0); } // endlos

/* func Initialize() { AddEffect("Potion",this(),100,1,0,PFIR); } */

public func Glow(iTime)   { return(RGB(
  255,
  182+Cos(iTime,15),
  182
  ));
}

public func Spark() { return(RGBa(255,182,182,RandomX(170,200))); }

public func Light(iTime) { return(RGBa(RandomX(240,255),RandomX(170,190),182,RandomX(180,200))); }
public func LightSpeed() { return(10); }
public func LightSize()  { return(30); }

protected func Activate(object pClonk) {
  // Sound
  Sound("Drink");
  // Aktion
  pClonk->~Drink(this());
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"LovePotion\",this(),160,15,0,PROS,30000,18000,50000,PROS->EffectDuration())",20,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxWipfPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  return(0);
}


protected func FxLovePotionStart(object pTarget, int iEffectNumber, int iTemp, int iChange0, int iChange1, int iChange2, int iTime)
{
  if(!iTemp) {
    EffectVar(9,pTarget,iEffectNumber) = iTime;
    AddEffect("Obj3D",pTarget,300,1,0,PFIR,PROS);
  }
}

protected func FxLovePotionTimer(object pTarget, int iNumber, int iTime) {
  if(!Contained(pTarget) || GetID(pTarget) == BARL)
  {
    var iRndX=RandomX(-5,5);
    CreateParticle("Heart", GetX(pTarget)+iRndX, GetY(pTarget)+RandomX(-10,-16),GetXDir(pTarget)/2, -RandomX(4,7),RandomX(50,70), RGBa(255,182,182+Random(30),Random(50)),pTarget);
  }  
  // Zeit abgelaufen!
}

private func FxLovePotionEffect(string szNewEffectName) { if(szNewEffectName eq "WipfPotion") return(-2); }

protected func FxLovePotionStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return();

  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == PROS)
      { RemoveEffect(0,pTarget,num); break; }
}
