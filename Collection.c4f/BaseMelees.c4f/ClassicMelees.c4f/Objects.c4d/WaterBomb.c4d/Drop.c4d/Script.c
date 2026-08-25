/*-- Tropfen --*/

#strict

protected func Initialize()
{
  if(GetMaterial(0,2)==Material("Acid")) SetColorDw(RGBa(Random(40),RandomX(160,240),Random(40),Random(80)));
  else if(GetMaterial(0,2)==Material("Lava") || GetMaterial(0,2)==Material("DuroLava")) SetColorDw(RGBa(RandomX(190,255),RandomX(200,240),Random(40),Random(80)));
  else if(GetMaterial(0,2)==Material("Oil")) SetColorDw(RGBa(RandomX(20,50),RandomX(20,50),1,Random(80)));
  else SetColorDw(RGBa(Random(40),RandomX(15,55),RandomX(100,180),Random(80)));
  SetYDir(-Random(28)-15);
  return(1);
}

protected func Hit()
{
  RemoveObject(); 
  return(1);
}