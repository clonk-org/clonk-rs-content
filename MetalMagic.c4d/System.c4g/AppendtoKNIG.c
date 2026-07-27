/*-- AppendtoKNIG --*/

#strict 2
#appendto KNIG

protected func QueryCatchBlow(pObj) 
{ 
  // Schutzfaktor: lässt Objekte zurückprallen 
  var iPower = BoundBy(GetMass(pObj), 0, 50) * Distance(GetXDir(pObj), GetYDir(pObj)) * Distance(GetXDir(pObj), GetYDir(pObj));   
  if(Random(50000) + iPower / 3 < IsShielded() * 1000 && ~GetOCF(pObj) & OCF_Alive) 
  {           
    // Projektil abprallen lassen 
    if(GetXDir(pObj) < 0) if(GetX(pObj) < GetX() + 6) SetPosition(GetX() + 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    if(GetXDir(pObj) > 0) if(GetX(pObj) > GetX() - 6) SetPosition(GetX() - 9, GetY(pObj) - GetYDir(pObj) / 3, pObj); 
    SetXDir(BoundBy(-GetXDir(pObj) / 3, -10, 10), pObj); SetYDir(0, pObj); 
    // Sounds dazu 
    if(GetMass(pObj) >= 10 && HasArmor()) Sound("ClonkHit*"); 
    if(GetMass(pObj) < 10 && HasArmor()) Sound("ArrowRicochet*"); 
    ProtectedCall(pObj,"Hit");       
    return 1; 
  } 
  //Leider nicht..
  //return _inherited(pObj); 
} 

public func GetBowOut(pBow, iAngle, fDeviate, &iOutX, &iOutY, &iOutR, &iOutXDir, &iOutYDir, &iOutRDir) 
  {
  // Zufallsabweichung
  if(!GetEffect("SniperPotion",this))
  if(fDeviate) 
    { 
    var iDeviation = GetAimingDeviation(pBow);
    iAngle += Random(iDeviation * 2 + 1) - iDeviation;
    }
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 70);
  iOutYDir = -Cos(iAngle, 70);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  // Fertig 
  return 1;
  } 

protected func ControlThrow()
{
  // Beim Laden nicht den Bogen wegwerfen
  if(IsLoading()) return 1;
  // Reiten und Fallenlassen
  if (IsRiding())
    if (Contents(0) && GetPlrDownDouble(GetOwner()))
    {
      SetCommand(this, "Drop");
      return 1;
    }
  // Keine überladene Steuerung
  return _inherited();
}


private func FindEnemyUnit() 
{ 
  var pObj; 
  // Nächstgelegene Mannschaftsobjekte überprüfen 
  while (pObj = FindObject(0, 0,0,-1,-1, OCF_CrewMember, 0, 0, 0, pObj)) 
    // Verfeindet? 
    if (Hostile(GetOwner(),GetOwner(pObj))) 
      // Nicht unsichtbar?
      if(!pObj->~IsHidden())
        // Feind gefunden 
        return pObj; 
  // Keine feindliche Einheit gefunden 
  return 0; 
}

// Befehle
public func ContextAttack(pClonk) 
{                           
  [Condition=Never] 
  return _inherited(pClonk); 
} 
 
public func ContextDefend(pClonk) 
{                           
  [Condition=Never] 
  return _inherited(pClonk);
}

public func ContextGuard(pClonk) 
{                           
  [Condition=Never] 
  return _inherited(pClonk);;
} 
 
public func ContextArm(pClonk) 
{                           
  [Condition=Never] 
  return _inherited(pClonk);
}

public func ContextOrder(pClonk)
{
  [Befehle|Image=KMIA]
  CreateMenu(KMIA, pClonk, pClonk, 0, "Befehle", 0, 1);
  if(IsNotFullyArmed()) AddMenuItem("%s", Format("ContextArm(Object(%d))", pClonk), KMIA, pClonk);
  AddMenuItem("%s", Format("ContextGuard(Object(%d))", pClonk), MI_G, pClonk);
  AddMenuItem("%s", Format("ContextDefend(Object(%d))", pClonk), MI_D, pClonk);
  AddMenuItem("%s", Format("ContextAttack(Object(%d))", pClonk), MI_A, pClonk);
}

public func Never() {}

/* Pfeil aus dem Inventar nehmen */ 
public func GetArrow(idID)
  { 
  // Einzelne Pfeile suchen 
  var pObj, pArrow; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrow())
      // ID-Vergleich
      if(!idID || GetID(pObj)==idID)
        return pObj; 
  // Bei Pfeilpacks Pakete aufteilen 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrowPack())
     // ID-Vergleich 
     if(!idID || pObj->~UnpackTo()==idID)
      // Bolzen aus Paket verwenden
      if(pArrow = pObj->~GetItem()) return pArrow;
  // Keine Bolzen gefunden 
  return 0; 
  }

/* Schild anziehen */ 
public func WearShield(pObj, szGraphic)
{
  // Nur ein Schild gleichzeitig
  if(HasShield()) return;
  // Kein Objekt oder keine Grafik angegeben?
  if(!pObj || !szGraphic) return;
  // Grafik nicht vorhanden?
  if(!SetGraphics(szGraphic, this, GetID(), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics))
    // Stellt das Schild eine Grafik für mich bereit?
    if(!SetGraphics(Format("%i%s", GetID(), szGraphic), this, GetID(pObj), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics))
      return;
  // Angezogen? dann ID speichern und Objekt löschen
  idShield = GetID(pObj);
  RemoveObject(pObj);
  // Sound dazu!
  Sound("Connect");
  return 1;
}


// Erst 25 Frames nach dem ControlDown-Kommando 
private func StopFighting() 
{ 
  if(GetEffect("*Stun*", this)
  || GetEffect("*ForcesToFightWith*", GetActionTarget(0)))
  {
  	ScheduleCall(this, "StopFighting", 10);
  	return;
  }
  
  // ComDir auf NONE setzen, damit der Clonk nach dem Kampfabbruch nicht weiterlaeuft. 
  SetComDir(COMD_None); 
  // StopFighting-Effekt anfuegen, der den Kampf nach 25 Frames abbricht, wenn nicht schon vorhanden. 
  if(!GetEffect("IntKNIGStopFighting",this(),0,1) ) AddEffect("IntKNIGStopFighting", this(), 1, 25, this() ); 
} 
 
protected func FxIntKNIGStopFightingStop() 
{ 
  // Kaempfen wir noch? 
  if(IsFighting() ) 
  { 
    // Letzten Gegner fuer weitere 25 Frames speichern 
    AddEffect("IntKNIGLastEnemy", this(), 1, 25, this(), 0, GetActionTarget() ); 
    // Kampf abbrechen 
    SetAction("Walk"); 
  } 
} 



func RejectFight(pEnemy)
{
	if(SpecialEffectCall("OnStartFight", pEnemy))return 1;
	 // Auch nicht, wenn ein entsprechender Effekt auf dem Clonk liegt
 	 if(GetEffect("*RejectingFights*", this)) return true;
	return _inherited(pEnemy, ...);
}
