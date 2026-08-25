/*-- Fundamente unzerstörbar --*/

#strict 2

#appendto BAS7

/* Schaden / Zerstörung */

protected func Damage(iChange, iByPlayer)
{
  if(GetID() != BasementID() ) return _inherited(iChange, iByPlayer);

  return;
}
