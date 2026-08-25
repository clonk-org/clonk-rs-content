/*-- Eisiger Wind --*/

#strict

// Todo: Sichtbarer Effekt, vielleicht mit Gamma bläulich machen

protected func Activate()
{
  Sound("Wind2", true);
  AddEffect("IcyWindCurse", 0, 130, 5, 0, _IWD);
  return(RemoveObject());
}

/* Effekt: IcyWindCurse */

protected func FxIcyWindCurseStart(object pTarget, int iEffectNumber, int fTemp, Var1, Var2, Var3, Var4)
{
  EffectVar(0, pTarget, iEffectNumber) = 0;
  EffectVar(1, pTarget, iEffectNumber) = 70;
  EffectVar(2, pTarget, iEffectNumber) = 128;
  EffectVar(3, pTarget, iEffectNumber) = 190;
  EffectVar(4, pTarget, iEffectNumber) = 255;
  EffectVar(5, pTarget, iEffectNumber) = 200;
}

protected func FxIcyWindCurseTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var  low=EffectVar(0,pTarget,iEffectNumber), avg=EffectVar(2,pTarget,iEffectNumber), hgh=EffectVar(4,pTarget,iEffectNumber);
  var lowt=EffectVar(1,pTarget,iEffectNumber),avgt=EffectVar(3,pTarget,iEffectNumber),hght=EffectVar(5,pTarget,iEffectNumber);

  var chg = 5+Random(10); if(low>lowt) chg *= -1; low = BoundBy(low+chg, 0, 255);
  var chg = 5+Random(10); if(avg>avgt) chg *= -1; avg = BoundBy(avg+chg, 0, 255);
  var chg = 5+Random(10); if(hgh>hght) chg *= -1; hgh = BoundBy(hgh+chg, 0, 255);

  EffectVar(0,pTarget,iEffectNumber)=low; EffectVar(2,pTarget,iEffectNumber)=avg; EffectVar(4,pTarget,iEffectNumber)=hgh;

  SetGamma(RGB(0,0,low),RGB(128,128,avg),RGB(255,255,hgh),4);

  if(iEffectTime>900)
  {
    EffectVar(1, pTarget, iEffectNumber) = 0;
    EffectVar(3, pTarget, iEffectNumber) = 128;
    EffectVar(5, pTarget, iEffectNumber) = 255;
  }

  if(iEffectTime>1000) return(-1);
}

protected func FxIcyWindCurseStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  SetGamma(RGB(0,0,0),RGB(128,128,128),RGB(255,255,255),4);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("44"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
