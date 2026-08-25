/*-- Kann ohne geschossen zu werden explodieren --*/

#strict
#appendto XARW

protected func Hit() {
  if (Armed) {
  	Explode(Random(5) + 10);
  	return(1);
  }
  Explode(Random(5)+8);
  return(1);
}
