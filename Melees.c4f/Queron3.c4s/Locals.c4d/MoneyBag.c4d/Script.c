/*-- Geldsack --*/

#strict

local m_val;
local iTimer;

protected func Entrance(object pContainer)
{
	if (GetOCF(pContainer)&OCF_CrewMember && GetOCF(pContainer)&OCF_Alive)
	{
		var val = GetValue();
		Sell(GetOwner(pContainer));
		pContainer->CastMsg(Format("+%d {{MBAG}}", val), 0xffff00);
	}
}
