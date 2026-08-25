/*-- Honig --*/

#strict

protected func Initialize()
{
  SetClrModulation(RGBa(255,180,0,0));
}

protected func Hit()
{
  var x = RandomX(-2, 2);
  var y = RandomX(-2, 2);

  return(1);
}


/* Timer */
protected func Decay(){
  SetLocal(0,Sum(Local(0),+1));
  if (GreaterThan(Local(0),Random(10))) RemoveObject();
}
