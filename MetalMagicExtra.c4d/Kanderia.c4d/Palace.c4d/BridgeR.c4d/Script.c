/*-- Zugbrücke (rechts) --*/

#strict 2

#include CPBL

/* Konfiguration */

public func GetTowerType()  { return CPT3; }
public func GetTowerType2() { return PPT3; }

private func FindTower() { if(FindObject(GetTowerType(), -40,0)) return(FindObject(GetTowerType(), -40,0)); return(FindObject(GetTowerType2(), -40,0)); }
