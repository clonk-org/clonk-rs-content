/*-- Palme --*/

#strict 2

#include TREE
#include PLM1

// Etwas leichter zu fällen
private func TreeStrength() { return 100; }

/* Bewegung (Wind) */  

private func Breeze()
{
  if (Abs(GetWind()) < 50 + MotionThreshold) SetAction("Still");
  if (Abs(GetWind()) > 74 + MotionThreshold) SetAction("Storm");
}
    
private func Storm()
{
  if (Abs(GetWind()) < 75 + MotionThreshold) SetAction("Breeze");
}
