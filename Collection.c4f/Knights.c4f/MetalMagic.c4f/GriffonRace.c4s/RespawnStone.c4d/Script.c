/*-- Groﬂer Stein --*/

#strict 2

protected func Initialize()
{
  SetAction("Wait");
}

public func Activate(pClonk)
{
  if(GetAction()=="Wait") SetAction("Up");
  else if(GetAction()=="Down")
  {
    var iPhase = GetPhase();
    SetAction("Up");
    SetPhase(5-iPhase);
  }
  else Up();
  SetColorDw(GetColorDw(pClonk));
}

public func Up()
{
  // Effekte!
  CastParticles("MSpark", 20, 40, 0,-14, 30, 45, GetColorDw(), GetColorDw());
}

public func End()
{
  SetColorDw(RGB(255,255,255));
}
