/*-- Ballon Pack --*/

#strict 2

protected func Entrance(object pClonk)
	{
	// Nur für lebendige Teammitglieder
	if (GetOCF(pClonk)&OCF_CrewMember && GetOCF(pClonk)&OCF_Alive)
		AddEffect("IntControlBalloon", pClonk, 1,, this);
	}

public func FxIntControlBalloonControlUpDouble(object pTarget, int iNr)
	{
	// Bei zwei Ballonrucksäcken: Effekt mit höchster Nr machen lassen
	var i = GetEffectCount("IntControlBalloon", pTarget);
	while (i--)
		{
		if (GetEffect("IntControlBalloon", pTarget, i) > iNr) return 0;
		}
	// Nicht im Wasser
	if (GetAction(pTarget) == "Swim") return 0;
	// schon ein Ballon dran?
	if (FindObject2(Find_ID(BALO), Find_ActionTarget(pTarget)))
		{
		Sound("Error",, pTarget);
		return Message("$MsgOnlyOneBalloon$", pTarget);
		}
	var balloon = CreateObject(BALO);
	balloon->Lift(pTarget);
	return RemoveObject();
	}
