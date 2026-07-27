/*-- Drachenstatue --*/

#strict 2

protected func Initialize()
{
	SetAction("Waiting");
}

protected func BreatheFireCheck()
{
	if (!Random(5))
		{
 	 	SetAction("Rise");
		}
}

protected func BreatheFire()
{
	CreateFireBreath(this,40,GetDir(this)*70-35,-32,40);
	AddEffect("IntFireBreath", this, 1, 40, this);
}

protected func FxIntFireBreathStop(object pTarget, int iNr)
{
	SetAction("Lower");
}
