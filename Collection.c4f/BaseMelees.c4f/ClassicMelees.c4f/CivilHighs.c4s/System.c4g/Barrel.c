/*-- Kanonen Setup --*/

#strict
#appendto BARL

protected func SellTo()
{
  // Fasskonfiguration: Immer Verkauf zum gefüllten Ölfass
  if (BarrelConfiguration() & 4) return(OBRL);
  // Ansonsten beim Verkauf entleeren
  return(_inherited());
}