/* Kleines Kraftfeld (Objekt) */

#strict 2
#include FRCA

local is_dead;

public func GetForceFieldTime() { return 1000; }
private func CheckStuck() {}
public func GetMaxDamage() { return 40; }

static const FRCS_Radius = 45,
              FRCS_RadiusMargin = 6, // Halbe Dicke des regulaeren Effektbereiches
              FRCS_TopSolidWidth = 30, // Halbe Breite der oberen, soliden Kante
              FRCS_Strength = 10; // Rausschleuder-Geschwindigkeit

public func GetLightningDamage() { return GetMaxDamage(); }

protected func Timer()
{
}
