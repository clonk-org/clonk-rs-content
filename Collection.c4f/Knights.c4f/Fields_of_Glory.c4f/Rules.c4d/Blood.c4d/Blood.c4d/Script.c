/*-- BLUT --*/

#strict

protected func Initialize()
{
	SetClrModulation(RGBa(RandomX(100, 250), 0, 0, RandomX(0, 100)));
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
  if (GreaterThan(Local(0),Random(5))) RemoveObject();
}
