#strict

protected func Hit() {
  Sound("RockHit*");
}

public func Activate()
{
 var throneroom = FindObject2(Find_ID(ET8Z));
 throneroom->Teleport();
 RemoveObject();
 return(1);
}
