/*--- Wipfseife ---*/

#strict

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
  // Effekt starten (zeitverzögert)
  Schedule("AddEffect(\"WipfSoap\",this(),160,25,0,SOAP,30000,18000,50000,SOAP->EffectDuration())",20,0,pClonk);
  // fertig..
  RemoveObject();
  return(1);
}

private func FxWipfPotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  return(0);
}


protected func FxWipfSoapStart(object pTarget, int iEffectNumber, int iTemp, int iChange0, int iChange1, int iChange2, int iTime)
{
  if(!iTemp) {
    EffectVar(9,pTarget,iEffectNumber) = iTime;
    AddEffect("Obj3D",pTarget,300,1,0,PFIR,SOAP);
  }
}

protected func FxWipfSoapTimer(object pTarget, int iNumber, int iTime) {
  if(!Contained(pTarget) || GetID(pTarget) == BARL)
  {
    var iRndX=RandomX(-5,5);
    CreateParticle("Bubble", GetX(pTarget)+iRndX, GetY(pTarget)+RandomX(-0,-4),GetXDir(pTarget)/2, -RandomX(4,7),RandomX(30,50), RGBa(255,255,255,Random(50)),pTarget);
  }  
  // Zeit abgelaufen!
}

private func FxWipfSoapEffect(string szNewEffectName) { if(szNewEffectName eq "WipfPotion") return(-2); }

protected func FxWipfSoapStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(fTemp) return();

  for(var num,i=0; num=GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,num) == SOAP)
      { RemoveEffect(0,pTarget,num); break; }
}


protected func Hit()
{
  CreateParticle("Bubble", 0, -3, RandomX(-7,7), -7, RandomX(30,45), RGBa(234,202,154,30));
  return(1);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(160); }

