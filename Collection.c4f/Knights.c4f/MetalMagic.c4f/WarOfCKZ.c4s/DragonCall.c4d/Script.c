/* Drachenbeschwörung */

#strict 2

/* Aufruf */

public func Summoned(pClonk)
{
  var pDragon = CreateObject(DRGN, 0, 19);
  if(GetDir(pDragon)) SetPosition(GetX(pDragon)-19, GetY(pDragon));
  pDragon->SetAction("WakeUp");
  // Kontrolle übernehmen
  pDragon->ResetControl();
  pDragon->UpdateRideVertex();
  return RemoveObject();
}

// Kann mit einem Ritual beschworen werden
public func IsRitual()	 { return 1; }

// Nur für Kirchenlose
public func NeedChurch() { return -1; }
