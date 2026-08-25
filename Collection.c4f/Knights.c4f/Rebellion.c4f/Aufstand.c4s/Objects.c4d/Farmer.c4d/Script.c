/*-- Bauernclonk --*/

#strict

protected func Initialize() 
{
  return(SetAction("Init"));
}

private func Initialized()
{
  var pClonk = CreateObject(CLNK, 0,0, GetOwner());
  if(Contained())
    Enter(Contained(), pClonk);
  MakeCrewMember(pClonk, GetOwner());
  return(RemoveObject());
}
