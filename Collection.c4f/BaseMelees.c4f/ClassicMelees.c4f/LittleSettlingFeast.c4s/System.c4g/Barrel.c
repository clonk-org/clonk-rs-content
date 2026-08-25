/*-- Kanonen Setup --*/

#strict
#appendto BARL

protected func SellTo()
{
  // Fasskonfiguration: Gegebenenfalls Verkauf zum gefüllten Wasserfass
  if (BarrelConfiguration() & 4) return(GetID());
  // Ansonsten beim Verkauf entleeren
  return(_inherited());
}