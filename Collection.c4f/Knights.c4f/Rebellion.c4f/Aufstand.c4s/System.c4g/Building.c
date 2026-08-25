/* Unzerstörbare Gebäude */

// Billige Lösung, aber funktioniert!
// evtl. Nebeneffekte?

#strict

#appendto *

protected func Damage(int iChange, int iByPlayer)
{
  if(iChange>0)
    DoDamage(-iChange);
}

