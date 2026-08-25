/*-- Flachland --*/

#strict

func Initialize() {
  SetGamma(RGB(2,1,0),RGB(127,120,110),RGB(255,245,240));
  return(1);
}

// Vulkan soll DuroLava verwenden
public func VolcanoMaterial() { return(Material("DuroLava")); }
