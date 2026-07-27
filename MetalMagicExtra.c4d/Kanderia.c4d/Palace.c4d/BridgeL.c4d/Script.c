/*-- Zugbrücke (links) --*/

#strict 2

/* Konfiguration */

public func GetTowerType() { return CPT4; }
public func GetTowerType2() { return PPT4; }

private func FindTower() { if(FindObject(GetTowerType(), 50,0)) return(FindObject(GetTowerType(), 50,0)); return(FindObject(GetTowerType2(), 50,0)); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Raised");
}

public func IsAttachedTo(object pTarget)
{	
	return (pTarget == FindTower());
}		

/* Status */

public func IsOpen() { return GetAction() == "Lowered"; }
public func IsOpening() { return GetAction() == "Lower"; }
public func IsClosed() { return GetAction() == "Raised"; }
public func IsClosing() { return GetAction() == "Raise"; }

public func IsDown() { return IsOpen() || IsOpening(); }
public func IsUp() { return IsClosed() || IsClosing(); }

/* Burgteilfunktionen */

public func CastleChange()
{
	return DestructCheck();
}	

private func DestructCheck()
{
  if (!FindTower()) 
  	RemoveObject();  
  return 1;
}

/* Steuerung */

public func ControlUp()
{
  if (GetAction() == "Lowered")
    return SetAction("Raise");
  if (GetAction() == "Lower")
  {
  	var iPhase = GetPhase();
  	SetAction("Raise");
  	SetPhase(20 - iPhase);
  	return 1;
	}    
  return 0;
}
  
public func ControlDown()
{
  if (GetAction() == "Raised")
    return SetAction("Lower");
  if (GetAction() == "Raise")
  {
  	var iPhase = GetPhase();
  	SetAction("Lower");
  	SetPhase(20 - iPhase);
  	return 1;
	}    
  return 0;  
}
  
/* Anschlag */

protected func Hit()
{
	// Geräusch
  Sound("DrawbridgeHit");
  // Transferzone des Turms ändert sich
  var pTower = FindTower();
  if (pTower) pTower->UpdateTransferZone();
}
  
private func UpdateSolidMask()
{
  if (GetAction() == "Lower")
    SetSolidMask(GetPhase() * 60, 60, 60, 60, 0, 0);
  if (GetAction() == "Raise")
    SetSolidMask(1200 - GetPhase() * 60, 60, 60, 60, 0, 0);
  if (GetAction() == "Lowered")
    SetSolidMask(1200, 60, 60, 60, 0, 0);
  if (GetAction() == "Raised")
    SetSolidMask(0, 60, 60, 60, 0, 0);
}  
