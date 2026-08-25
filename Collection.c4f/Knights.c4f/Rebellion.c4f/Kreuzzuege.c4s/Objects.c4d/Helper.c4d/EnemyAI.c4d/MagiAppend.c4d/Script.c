/* Homewarp für KI-Zauberer */

#strict

#appendto MCLK

local ai_pTarget, ai_iRange, ai_pTarget2; // Redeklaration für Lokale Kompilierung...

public func DoSpellAim(object pSpell, x,y,z,a,b,c)
  {
  // Winkelziel-Callback
  // Normaler Spieler
  if (!ai_iRange) return(_inherited(pSpell, x,y,z,a,b,c));
  // KI: Zielt immer aufs gegenwärtige Ziel
  var pTarget = ai_pTarget;
  if (!pTarget) pTarget = ai_pTarget2;
  if (!pTarget) return(0);
  var iAngle = Angle(GetX(), GetY(), GetX(pTarget), GetY(pTarget));
  if (iAngle > 180) iAngle -= 360;
  pSpell->~AimingAngle(this(), iAngle);
  pSpell->ActivateAngle(this(), iAngle);
  return(true);
  }

public func DoSpellSelect(object pSpell, int iRange, x,y,z,a,b,c)
  {
  // Zielauswahl-Callback
  // Normaler Spieler
  if (!ai_iRange) return(_inherited(pSpell, iRange, x,y,z,a,b,c));
  // KI: Zielt bevorzugt auf sich; ansonsten immer aufs gegenwärtige Ziel, wenn möglich
  var pTarget = this();
  if (!pSpell->~SelectorTarget(pTarget))
    {
    if (!ai_pTarget) return();
    if (!pSpell->~SelectorTarget(pTarget = ai_pTarget))
      if (!pSpell->~SelectorTarget(pTarget = ai_pTarget2))
        return();
    if (ObjectDistance(pTarget) > iRange) return();
    }
  pSpell->ActivateTarget(this(), pTarget);
  return (true);
  }

global func Fling(object o, int vx, int vy, a,b,c,d)
  {
  // Feuerklumpen-Fling nicht auf KI
  if (GetID()==MFBL) if (LocalN("ai_iRange", o)) return();
  return(_inherited(o, vx, vy, a,b,c,d));
  }
