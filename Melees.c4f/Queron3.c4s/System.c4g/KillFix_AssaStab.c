/* Assa: Killverfolgung Stabkill, Keulen erzeugen keine Errors */

#strict 2

#appendto ASAS

// Keulenschläge erzeugen keine Errors

protected func FxSleepNSpellStart(object pTarget, int iEffectNumber, int iTemp) {
  // temporäre aufrufe
  if(iTemp) return;
  // Scheintoter Zustand
  ObjectSetAction(pTarget, "Dead");
  // Zeit bis zum Aufwachen speichern
  EffectVar(0,pTarget,iEffectNumber) = 250;
  // Fertig
  return 1;
}

protected func FxSleepNSpellStop(object pTarget, int iEffectNumber) {
  // clonk hat den Schlaf nicht überstanden ;)
  if(!GetAlive(pTarget)) return 1;
  // aufstehen
  ObjectSetAction(pTarget,"FlatUp",0,0,1);
  // stehenbleiben
  SetComDir(COMD_Stop,pTarget);
  // Markereffekt entfernen (für KI benötigt)
  RemoveEffect("IntFoundMarker", pTarget);
  // Fertig
  return 1;
}

// Killverfolgung für Stabkills

public func Assassasinated()
{
  var pTarget = GetActionTarget();
	if(pTarget && ObjectDistance(pTarget)<22 && pTarget != this) // mit extra Sicherheit dass sich der Assassine nicht selbst ersticht
  {
    if(pTarget->~HasArmor())
    {
      Sound("ClonkHit*");
      DoEnergy(-30, pTarget,,, GetOwner()); // + iCausedBy
    }
    else
    	pTarget->SetKiller(GetOwner());
      pTarget->Kill();
  }
}
