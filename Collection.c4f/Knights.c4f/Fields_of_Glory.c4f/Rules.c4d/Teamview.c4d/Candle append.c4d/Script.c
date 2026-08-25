#strict

#appendto CNDL

protected func Initialize()
{
  SetCategory(C4D_Vehicle());
  SetAction("Off");
  SetPlrViewRange(0);
}

protected func ControlDigDouble()
{
  if(GetAction() S= "On") return(SetOff());
  SetAction("On");
  SetPlrViewRange(10);  
}

public func SetOff()
{
  SetAction("Off");
  Sound("Pshshsh");
  SetPlrViewRange(0);  
  return(1);
}
