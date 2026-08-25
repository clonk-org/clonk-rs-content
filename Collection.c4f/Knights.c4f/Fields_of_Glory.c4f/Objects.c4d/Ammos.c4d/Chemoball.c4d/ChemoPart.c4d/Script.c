/*-- Bruchteil einer Chemokugel --*/

#strict
#include _BLP

func Activate()
{
  if (GetAction() eq "Activated") return(1);
  SetAction("Activated");
  return(1);
}

protected func Spreed()
{
  CastObjects(GAS_,1,5);
}

func Damage()
{
  if (GetDamage() < 1) return(1);
  SetAction("Activated2");
  return(1);
}

func Hit() { return(Sound("Fuse")); }