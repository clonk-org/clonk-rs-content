/*-- Fass --*/

#strict 2

protected func Initialize()
{
	SetAction("Barrel");
	Rand();
}

public func Rand()
{
	return Set(Random(GetActMapVal("Length", "Barrel", GetID())));
}

public func Set(int iPhase)
{
	SetPhase(iPhase);
	return 1;
}
