/*-- Airpedo --*/

#strict 2

public func Launch(int iAngle, object pClonk)
{
  SetR(iAngle + 90);
  var i = AddEffect("IntBlow", this, 1, 12, this);
  EffectVar(0, this, i) = pClonk;
  SetAction("Fly");
  AddEffect("IntAirpedo", this, 1, 1, this);
}

protected func FxIntAirpedoTimer(object pTarget, int iEffect)
{
  var speed = EffectVar(0, pTarget, iEffect) = BoundBy(EffectVar(0, pTarget, iEffect) + 1, 0, 100),
  angle = GetR(pTarget) - 90;
  SetXDir( Sin(angle, speed), pTarget, 5);
  SetYDir(-Cos(angle, speed), pTarget, 5);
}

public func NoCatchBlow(object pObj)
{
  return pObj == EffectVar(0, this, GetEffect("IntBlow", this));
}

protected func Hit()
{
  Explode(30);
}