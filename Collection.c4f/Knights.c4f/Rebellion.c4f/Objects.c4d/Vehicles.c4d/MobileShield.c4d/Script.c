/*-- Fahrbares Schutzschild --*/

#strict

protected func Initialize()
{
  SetAction("Ready");
}


protected func ContactLeft() 
{
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

protected func ContactRight() 
{
  if(Stuck() && !Random(5)) SetRDir(RandomX(-7, +7));
}

  
protected func UpdateSolidmask()
{
  SetSolidMask(54+27*GetDir(),0,27,30);
}

