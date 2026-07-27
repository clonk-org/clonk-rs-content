/*-- Burg-Werkstatt --*/

#strict 2

#include DOOR // Türsteuerung
#include WRKS // Werkstatt
#include PPW2 // Burgteilfunktion

/* Burgbauteil */

public func CastlePartWidth() { return (80); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return BAS8; }

protected func Initialize()
{
  // Ursprungsfunktion
  _inherited();
  // Amboss erzeugen
  CreateObject(ANVL, +13, +37, GetOwner());
}

/* Forschung */

public func GetResearchBase() { return PPW2; }

/* Türsteuerung */

private func SoundOpenDoor()
{
  Sound("SteelGate2");
}
  
private func SoundCloseDoor()
{
  Sound("SteelGate2");
}
