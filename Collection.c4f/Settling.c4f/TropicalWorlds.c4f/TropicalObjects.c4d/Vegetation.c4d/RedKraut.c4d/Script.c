/*-- Seegras --*/

#strict

#include SWD1


protected func Initialize()
{
  //SetAction("Wiggle");
  return(1);
}

private func ReproductionAreaSize() { return(120); }
private func ReproductionRate()     { return(22); }
private func MaxTreeCount()         { return(12); }

protected func Damage() { RemoveObject(this()); }
