/*-- Boje --*/

#strict

local iColor;

protected func Initialize()
{
  iColor=255;
  return(1);
}

protected func Hit()
{
  Sound("ClonkHit*");
  return(1);
}

func Damage()
{
  iColor=255-GetDamage();
  if(iColor<150) iColor=150;
  SetClrModulation(RGB(iColor,iColor,iColor));
}

public func IsAnvilProduct()
{
  return(1);
}

func CalcValue()
{
  return(10-(13-(iColor+5)/20));
}