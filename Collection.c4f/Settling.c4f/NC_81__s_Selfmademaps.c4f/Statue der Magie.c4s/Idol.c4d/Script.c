/*-- Statue der Magie --*/

#strict

protected func Hit3() {
  Sound("RockHit*");
  return(1);
}

protected func Initialize() {
  SetCategory(C4D_Vehicle());
  return(1);
}

