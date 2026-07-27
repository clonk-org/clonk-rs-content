/*-- Das Seilende --*/

#strict 2

local pPull, pRope, pHook;
                
public func SetRope (r, h) {
  pRope = r;
  pHook = h;
}

func Departure () {
  pPull = 10;
}
func Entrance () {
  pPull = 0;
}

// Steuerung
func ControlLeft (clonk) {
  pPull = 1;
  if(GetXDir(clonk)<5) SetXDir(GetXDir(clonk)-10, clonk);
  if(GetAction(clonk)=="Tumble") ObjectSetAction(clonk, "Jump");
}
func ControlRight (clonk) {
  pPull = 1;
  if(GetXDir(clonk)>-5) SetXDir(GetXDir(clonk)+10, clonk);
  if(GetAction(clonk)=="Tumble") ObjectSetAction(clonk, "Jump");
}
func ControlUp (clonk) {
  if (GetProcedure (clonk) == "FLIGHT") pPull = -1;
  else if (GetProcedure (clonk) == "WALK") pPull = 1;
  else pPull = 1;
}
func ControlDown (clonk) {
  pPull = 2;
}

public func Activate(pClonk) { [$LetRopeGo$] return ControlDigDouble(pClonk); }

func ControlDigDouble(clonk) {
  [$LetRopeGo$]
  pHook->SetAction("Idle");
  Enter (clonk, pHook);
  RemoveObject (pRope);
  RemoveObject ();
  return 1;
}

public func RopeBreak() { return RemoveObject(); }

func RopeAskChangeLength (len, obj) {
  if (obj != pRope) return;
  if (Contained (Contained ())) return ControlDigDouble (Contained ());
  // Ohne Kontrolle dem Wunsch des Seils nachgeben
  if (!Contained ()) return (len + 1);
  // Bei zu starkem Zug kann der CLonk das Seil nicht mehr festhalten
  if (pPull <= 0) if (len >= 15) if (ObjectCall (obj, "GetRopeLength") < 1000) return 1;
  // Nicht zu viel Seil lassen
  if (pPull >= 0) if (len <= -10) return -1;
  // Am Seil ziehen
  if (pPull < 0) if (len <= 3) return pPull;
  if (pPull < 0) if (len <= 6) return -1;
  // Seil geben, nicht zuviel
  if (pPull > 0) if (len >= 0) if (ObjectCall (obj, "GetRopeLength") < 1000) return pPull;
  return;
}
