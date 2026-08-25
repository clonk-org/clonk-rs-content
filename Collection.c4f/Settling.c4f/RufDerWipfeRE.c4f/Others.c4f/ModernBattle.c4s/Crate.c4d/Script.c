/*-- Crate --*/

#strict

func Hit2()
{
  Damage();
}

func Damage()
{
  if(Random(3))
  {
    CastParticles("Splinter", 10, 50, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
    Sound("CrateCrack");
    RemoveObject(this,true);
  }
}
