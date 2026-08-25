/*--- Der Stein ---*/

#strict

local alpha;

protected func Initialize()
{
  SetAction("Rock");
  SetPhase(Random(3));
  SetDir(Random(2));
  alpha = -200;
}

private func FadeOut()
{
  alpha+=10;
  if(alpha>240) RemoveObject();
  SetClrModulation(RGBa(255,255,255,BoundBy(alpha,0,255)));
}

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

