/*-- Super-T-Flint --*/

#strict

#include TFLN

public func ExplodeSize() { return(27); }

/* Forschung */

public func GetResearchBase() { return(TFLN); }

protected func Hit() {
  Sound("RockHit*");
  if(!ActIdle()) return BlowUp();
  Sound("Fuse");
  SetAction("Activated");
  // Picture anpassen
  SetPicture(0, 12 + 64, 64, 64);
  return 1;
}
