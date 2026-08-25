/*-- Zapmaster --*/

#strict

protected func Hit(){Sound("Fuse");}

public func Activate(pClonk)
{
  if (pClonk->IsZapmaster()) return(1);
  pClonk->BuckleCap();
  Sound("Fuse");
  RemoveObject();
  return(1);
}

/* Kann geschmiedet werden */
protected func IsAnvilProduct() { return(1); }