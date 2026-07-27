/* Flagge: Wechsel des Besitzers mit Einsammeln */

#strict 2

#appendto FLAG

protected func Entrance(object pClonk)
	{
	if (GetOCF(pClonk) & OCF_CrewMember) SetOwner(GetOwner(pClonk));
	}
