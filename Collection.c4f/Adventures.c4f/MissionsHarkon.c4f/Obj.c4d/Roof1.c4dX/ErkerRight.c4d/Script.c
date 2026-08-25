/*-- Wand --*/

#strict 2
#include CPT2 // Burgteil - Treppenhaus - Turm
/* Initialisierung */

protected func Initialize()
{
  // Tür ist immer offen
  SetEntrance(1);
  // Transferzone
  UpdateTransferZone();
  //Stuhl erstellen
  var chair= CreateObject(_CHA, 27, 27,GetOwner() );
  chair -> SetDir(DIR_Right);
  // Basisimplementation
  _inherited();
  // Fertig
  return 1;
}

/* Bei Veränderung der Burg */

public func CastleChange()
{
  UpdateTransferZone();
}

/* Keine Tore, Transfer des Treppenhauses */

private func InitializeTower() { return(1); }

public func UpdateTransferZone()
  {
  // Mit darüberliegendem Niedergang
  if (FindStaircaseAbove())
    return(SetTransferZone(-8,-47,16,61));
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return(SetTransferZone());
  }

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (40); }
public func CastlePartHeight() { return (62); }
public func CastlePartTop() { return (-28); }
public func CastlePartBasement() { return 0; }

/* Forschung */

public func GetResearchBase() { return(CPT2); }
  