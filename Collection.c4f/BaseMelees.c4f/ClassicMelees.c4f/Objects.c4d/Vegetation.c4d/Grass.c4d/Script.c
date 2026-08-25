/*-- Grass --*/

#strict 2
#include _COV

protected func Initialize() 
{ 
  // Zufällige Größe
  if (Random(5))
    DoCon(-Random(20) - 40);
  else
    DoCon(-Random(50) - 20);
  // Zufällige Form  
  SetAction("Grass");
  SetPhase(Random(2));
  if (!Random(20)) SetPhase(2);
  // Zufällige Richtung
  if (Random(2)) SetDir(DIR_Right);
  // Drehung nach Erdoberfläche
  var x_off = 18 * GetCon() / 100;
  var y_off = 15 * GetCon() / 100;
  var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
  SetR(slope);
  // Gras bleibt hinter Bäumen
  MoveBehindTrees();
}