/*-- Kameraobjekt --*/

#strict

protected func Initialize()
{
  SetPosition(LandscapeWidth()/2,1);
  SetName("", this());
}

protected func Recruitment(iPlr)
{
  SetCrewEnabled(false);
  SetPlrViewRange(0);
  SetName("", this());
  return(1);
}

