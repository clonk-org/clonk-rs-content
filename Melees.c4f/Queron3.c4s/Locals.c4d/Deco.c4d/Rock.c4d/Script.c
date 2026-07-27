/*-- Fels --*/

#strict 2

protected func Initialize()
{
	SetAction("Rock");
	Rand();
}

public func Rand()
{
	return Set(Random(GetActMapVal("Length", "Rock", GetID())));
}

public func Set(int iPhase)
{
	SetPhase(iPhase);
	var w = GetDefWidth();
	SetSolidMask(iPhase*w, 0, w, w);
	return 1;
}

protected func Damage()
{
  if (GetDamage() > 50)
  {
		Split2Components();
		var s = GetDefWidth();
		// Stauuuuuubbbb *HUST*
		for (var i=0; i < 10; i++)
			CreateParticle("Dust", RandomX(-s/2, s/2), RandomX(-s/2, s/2), RandomX(-2, 2), RandomX(-2, 2), 5*RandomX(10, 40));
		return 1;
	}
}
