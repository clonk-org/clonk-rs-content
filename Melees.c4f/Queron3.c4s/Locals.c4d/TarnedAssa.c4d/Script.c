/* Getarnter Assa */

#strict 2

#appendto ASAS

local fHide;

// Kein Alpha-Hack

public func DoShow()
{
  var i = _inherited(...);
  if (GetEffect("Hide", this))
    EffectCall(this, GetEffect("Hide", this), "Timer", GetEffect("Hide", this, 0, 6));
}

func FxHideStart(pTarget, iNumber, fTmp, fInstantHide)
{
  if (GetEffect("Freeze", pTarget)) return -1; // nicht bei Eis
  return _inherited(pTarget, iNumber, fTmp, fInstantHide, ...);
}

func FxHideTimer(pTarget, iNumber)
{
  // Transparenter werden
  if(EffectVar(3, pTarget, iNumber)<235)
  { 
    var iChange = Min(235-EffectVar(3, pTarget, iNumber), 20);
    EffectVar(3, pTarget, iNumber) += iChange;
    if (EffectVar(3, pTarget, iNumber) == 235) // Bei Alpha=235 voll getarnt
    {
      SetGraphics(0, pTarget, _TAS); // extra Grafik
      SetClrModulation(RGBa(255, 255, 255), pTarget); // dafür kein Alpha
    }
    else
    {
      SetGraphics(0, pTarget, ASAS);
      SetClrModulation(RGBa(255,255,255,EffectVar(3, pTarget, iNumber)));
    }
  }
  SetColorDw(GetPlrColorDw(GetOwner(pTarget)), pTarget);
  // Partikeleffekt
  EffectVar(2, pTarget, iNumber) += 40;
  if(EffectVar(2, pTarget, iNumber)==360) EffectVar(2, pTarget, iNumber) = 0;
  EffectVar(1, pTarget, iNumber)->CreateParticle("HideSpark", Sin(EffectVar(2, pTarget, iNumber), 20), Cos(EffectVar(2, pTarget, iNumber), 20),-Sin(EffectVar(2, pTarget, iNumber), 2),-Cos(EffectVar(2, pTarget, iNumber), 2), 30, GetPlrColorDw(GetOwner(pTarget)), EffectVar(1, pTarget, iNumber));
}

func FxHideStop(pTarget, iNumber, fTmp)
{
  // Alpha auf alten Wert setzen
  SetClrModulation(RGBa(255, 255, 255));
  SetGraphics(0, pTarget, ASAS);
  SetColorDw(GetPlrColorDw(GetOwner(pTarget)), pTarget);
  // Walk zurücksetzten
  ResetPhysical(pTarget, "Walk");
  ResetPhysical(pTarget, "Scale");
  ResetPhysical(pTarget, "Hangle");
  ResetPhysical(pTarget, "Energy");
  SetPhysical("Energy", GetPhysical("Energy", 1, pTarget)/2, 2, pTarget);
  // Hilfsobjekt entfernen
  RemoveObject(EffectVar(1, pTarget, iNumber));
  // Kategorie zurücksetzten
  SetCategory(EffectVar(4, pTarget, iNumber));
  fHide = 0;
}
