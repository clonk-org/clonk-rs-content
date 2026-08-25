/*-- Sandsackbarriere --*/

#strict

public func Initialize()
{
  for(var obj in FindObjects(Find_OCF(OCF_CrewMember),Find_OCF(OCF_Alive),Find_Distance(20),Find_Func("Stuck")))
  {
    SetPosition(GetX(obj),GetY(obj)-10,obj);
    if(Stuck(obj))
      SetPosition(GetX(obj),GetY(obj)+10,obj);
  }
}

protected func Incineration()
{
  // Kaputt!
  CastParticles("Sandbag", 10, 100, 0,0, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  RemoveObject();
}

public func BasicalConstruction()
{
  return true;
}
