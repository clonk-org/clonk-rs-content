/*-- Airpedo --*/

// script v1.0 mod by miniwipf
#strict

/* Initialisierung */

protected func Departure(object pClonk)
{
	Launch(pClonk);
	return 1;
}

protected func Initialize() {
  SetAction("Remain");
  SetComDir(COMD_Stop());
}

public func Launch(object pBy) {
  SetAction ("Fly");
  var dir = GetDir(pBy);
  SetDir(dir);
  if (dir == DIR_Right()) SetComDir(COMD_Right());
  if (dir == DIR_Left()) SetComDir(COMD_Left());
  //SetXDir(GetXDir(by));
 // SetYDir(0);
  return(1);
}

/* Einsammeln */

func Entrance (object container) {
  SetAction("Remain");
}

/* Effekte */

private func Flying() {
	// Flug horizontal stabilisieren
	var ydir = GetYDir();
	if (ydir < 0) SetYDir(ydir+4);
	if (ydir > 0) SetYDir(ydir-4);
	// Und beschleunigen
  var dirX=(GetDir()*2-1);
  SetXDir((dirX*3)+GetXDir());
  Bubble(-16*dirX,0);
  Bubble(-16*dirX,0);
  //if (GetActTime() > 25) SetYDir(0);
}

/* Aufschlag */

public func Hit() { Explode(30); }

/* Produkteigenschaften */

public func IsChemicalProduct() { return(1); }

/* Status */

public func IsTorpedo() { return(1); }

/* Forschung */

public func GetResearchBase() { return(ARPD); }

