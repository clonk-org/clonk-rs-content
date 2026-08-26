#strict

public func Activate(oCaller, oClonk)
{                            
	Sound("Fuse");
	SetAction("Fuse");
	return(1);
}

public func Boom()
{
	Sound("Blast3");
	Explode(50);
}