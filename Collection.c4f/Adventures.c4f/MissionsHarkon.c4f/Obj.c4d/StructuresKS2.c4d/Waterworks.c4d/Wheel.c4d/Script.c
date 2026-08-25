/*-- Rad --*/

#strict 2

protected func TurningWheel()
{
  // Nach einiger Zeit...
  if(GetActTime()>=180)
  {
    // ... Wasser machen und Zeit zurücksetzen
    WaterProduced();
    SetAction("Idle"); SetAction("Wheel");
  }
}

protected func WaterProduced()
{
  // Leeren Eimer finden
  var pBucket = FindContents(KEBT);
  if(!pBucket) return;
  // und füllen!
  pBucket->~Fill();
  return 1;
}

public func StartWheel()
{
  if(GetAction() == "Wheel") return;
  var iPhase = GetPhase();
  SetAction("Wheel");
  SetPhase(iPhase);
}

public func StopWheel()
{
  if(GetAction() == "Stand") return;
  var iPhase = GetPhase();
  SetAction("Stand");
  SetPhase(iPhase);
}

protected func ControlThrow(object pClonk)
{
  // Herausnehmen per Graben: Holen-Menü öffnen
  if(RejectCollect(GetID(Contents(0, pClonk)),Contents(0, pClonk)))
    pClonk->SetCommand(0, "Get", this(), 0,0, 0, 1);
}

public func RejectCollect(idObj, pObj)
{
  // Nur Eimer oder Ehre aufnehmen
  if(idObj == KBKT || idObj == KEBT || idObj == EHRE) return 0;
  return 1;
}