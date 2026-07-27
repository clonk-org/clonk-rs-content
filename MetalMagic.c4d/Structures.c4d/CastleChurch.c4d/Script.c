/*-- Hofkapelle --*/

#strict 2
#include CHPL // Kapelle
#include CPW2 // Burgteilfunktion


/* Initialisierung */

protected func Initialize() {
  // Bei Fertigstellung an anderen Teilen ausrichten
  CastlePartAdjust();
  // In den Hintergrund versetzen
  SetCategory(C4D_StaticBack);
  // Verzögerte Initialisierung
  SetAction("Initialization");
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
  // Fertig
  return 1;
  }

/* Damit die anderen Teile bescheid wissen */
protected func Destruction() {
  DefinitionCall(CPPL, "CastlePartDestruction", this);
  return 1;
}

private func Initialize2() {
  // Fackeln erzeugen (verzögert, für korrekte Sortierung)
  ObjectCall(SetVar(0,CreateObject(TOC2,-26,+11)),"Activate");
  ObjectCall(SetVar(1,CreateObject(TOC2,+29,+11)),"Activate");
  SetDir(DIR_Right,Var(1));
  return 1;
  }

/* Türsteuerung */

protected func ActivateEntrance(pClonk) {
  if (Not(ActIdle())) return 1;
  // nur Verbündeten und eigenen Clonks öffnen
  if (Hostile(GetOwner(),GetOwner(pClonk))) return 0;
  SetAction("OpenDoor");
  Sound("GateOpen");
  return 1;
  }

private func OpenEntrance() {
  SetEntrance(1);
  return 1;
  }

private func CloseEntrance() {
  // Schließen
  SetAction("CloseDoor");
  SetEntrance(0);
  Sound("GateClose");
  return 1;
  }

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (80); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return BAS8; }

/* Forschung */

public func GetResearchBase() { return CPOF; }
