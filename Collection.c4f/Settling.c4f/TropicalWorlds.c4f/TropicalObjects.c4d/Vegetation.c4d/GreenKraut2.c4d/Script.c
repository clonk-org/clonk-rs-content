/*-- Seegras --*/

#strict

#include SWD1


protected func Initialize()
{
  //SetAction("Wiggle");
  return(1);
}

private func ReproductionAreaSize() { return(120); }
private func ReproductionRate()     { return(18); }
private func MaxTreeCount()         { return(15); }

protected func Damage() { RemoveObject(this()); }
