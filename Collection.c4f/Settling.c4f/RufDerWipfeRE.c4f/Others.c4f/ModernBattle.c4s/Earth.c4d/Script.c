/*--- Erde ---*/

#strict

public func SelfDestroy()
{
  if(!Random(4))
    return Hit(GetXDir()*10,GetYDir()*10);
}

protected func Hit(iXDir,iYDir)
{
  CastPXS("Earth", 100, (Abs(iXDir)+Abs(iYDir))/20);
  RemoveObject();
  return(1);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(120); }