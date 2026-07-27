/* Schriftrolle: Kosten je nach Zauber */

#strict 2

#appendto SCRL

local idSpell;

protected func CalcValue()
{
	if (GetSpellRank(idSpell)) return GetSpellRank(idSpell);
  return 1;
}
