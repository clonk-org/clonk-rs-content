/*-- Relaunchobject --*/

#strict 2

func Initialize() {
   SetAction("Fly");
   SetYDir(-3);
  return 1;
}

protected func RejectCollect() { return 1; } // Damit man nichts in das Objekt ablegen kann
