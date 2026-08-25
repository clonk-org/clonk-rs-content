/*-- Aquaclonk --*/

#strict 2

#include CLNK

private func Picking()
{
  if (!GetActTime()) return; // Erster Schlag kein Sound. Clonk holt noch aus.
  Sound("MetalHit*");
  CastParticles("MaterialSpark",Random(3)+1,6,-10+20*GetDir(),6,50,62, RGB(100,100), RGB(100,100));
  return 1;
}