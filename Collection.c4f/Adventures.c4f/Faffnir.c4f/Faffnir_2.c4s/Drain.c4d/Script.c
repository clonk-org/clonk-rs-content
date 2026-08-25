/*-- Drain --*/

#strict

protected func Initialize()
{
  SetAction("Drain");
  return(1);
}

private func Draining()
{
  if(GetMaterial() == Material("Water"))
    ExtractMaterialAmount(0, 0, GetMaterial(), 20);
  return(1);
}
