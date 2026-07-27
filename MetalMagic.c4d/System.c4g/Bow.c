/*-- Bogen --*/

#strict 2

#appendto BOW1

// local Local(9); Local(9);
// local Local(10); Local(10);

/* Aktivieren (zum Zielen per Maussteuerung) */
public func Activate(pClonk)
{
  [$CtrlActivateDesc$|Condition=ShowInContextMenu]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return 0;
  // Träger nicht bereit zum neuen Zielen
  if (!pClonk->~ReadyToFire())
  {
    if(pClonk->GetProcedure() != "SCALE") return 0;
    // Beim Klettern nur nach Doppelstop ablegen
    if(!GetPlrDownDouble(pClonk->GetOwner()))
      return 1;
    else
      return 0;
  }
  // Träger: neue Zielaktion setzen
  if (!pClonk->~LoadBow(1))
  {
    if (!pClonk->~AimBow()) 
      return;
    SetPhase(7, pClonk);
  }
  // Munitionsid suchen
  Local(9) = GetMunitionID();
  Local(10) = 0;
  if(Local(9)) PlayerMessage(GetOwner(pClonk), "%s", this, GetName(0,Local(9)));
  // Erfolg
  return 1;
}

public func ControlDig(pClonk)
{
  if (IsAiming())
  {
    // Nächste Munition
    Local(10)++;
    Local(9) = GetMunitionID(Local(10));
    if(Local(9)) PlayerMessage(GetOwner(pClonk), "%s", this, GetName(0,Local(9)));
    return 1;
  }
  return 0;
}

/* Munitionssuche */
private func GetMunitionID(iIndex)
{
  if(!Contained()) return;
  var aAmmo = [], idTest, idFound, fFound;
  // Liste mit Munitionsid's machen
  for(var pObj in FindObjects(Find_Container(Contained()), Find_Or(Find_Func("IsArrow"), Find_Func("IsArrowPack"))))
  {
    // ID suchen
    idFound = GetID(pObj);
    if(pObj->~IsArrowPack()) idFound = pObj->~UnpackTo();
    // Haben wir die ID schon?
    fFound = 0;
    for(idTest in aAmmo)
      if(idTest == idFound) fFound = 1;
    if(!fFound) aAmmo[GetLength(aAmmo)] = idFound;
  }
  // den Eintrag mit passendem Index ausgeben
  return aAmmo[ Mod(iIndex, GetLength(aAmmo)) ];
} 

// Haben wir noch was von der Munitionssorte?
private func CheckMunitionID(idID)
{
  // Liste mit Munitionsid's machen
  var idFound;
  for(var pObj in FindObjects(Find_Container(Contained()), Find_Or(Find_Func("IsArrow"), Find_Func("IsArrowPack"))))
  {
    // ID suchen
    idFound = GetID(pObj);
    if(pObj->~IsArrowPack()) idFound = pObj->~UnpackTo();
    // Ist die ID die gesuchte?
    if(idFound == idID) return 1;
  }
}

/* Pfeil abfeuern */
public func Fire()
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return 0;
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return 0;
  // Noch beim Nachladen
  if (GetAction() == "Reload") return 0;
  // Pfeil vom Träger holen
  var pArrow = pClonk->~GetArrow(Local(9));
  if(!pArrow)
  {
  	Local(9)=GetMunitionID();
  	Local(10)=0;
  	pArrow = pClonk->~GetArrow(Local(9));
  }
  if (!pArrow) return 0;
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(pClonk, 0, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iAngle = pClonk->~GetBowAimAngle();
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this, iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutOld(pClonk, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
  pClonk->~Control2Effect("ShootProjectile", pArrow);
  // Sicht verfolgen
  if (GetPlrView(GetController(pClonk)) != 2)
    SetPlrView(GetController(pClonk), pArrow);
  // Sound
  Sound("Bow");
  // Nachladen
  SetAction("Reload");
  pClonk->~LoadBow();
  // Munitionscheck
  if(!CheckMunitionID(Local(9)))
  {
    // Leer? Dann andere Munition
    ControlDig(Contained());
  }
  // Fertig!
  return 1;
}


/* Pfeil abfeuern */
public func FireAngle(iAngle)
{
  var pClonk = Contained();
  // Kein Träger?
  if(!pClonk) return(0);
  // Im Gebäude?
  if (pClonk->Contained()) { pClonk->~StopAiming(); return(0); }
  // Unvollständig?
  if (~GetOCF() & OCF_Fullcon) return(0);
  // Noch beim Nachladen
  if (GetAction() == "Reload") return(0);
  // Pfeil vom Träger holen
  var pArrow = Contained()->~GetArrow();
  if (!pArrow) return;
  // Beim Träger ggf. Bogen wieder auswählen
  ShiftContents(Contained(), 0, BOW1);
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
  if (!pClonk->~GetBowOut(this(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir))
    GetBowOutAngle(Contained(), iAngle, true, iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir);
  // Besitzer des Projektils setzen
  pArrow->SetOwner(Contained()->GetOwner());
  pArrow->SetController(Contained()->GetOwner());
    // Pfeil abfeuern
  Exit(pArrow, iOutX,iOutY,iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  
  var i=Contained()->~SpecialArrowTreatment(pArrow);
  pClonk->~Control2Effect("ShootProjectile", pArrow);
  pArrow->~Launch();
    // Sound
  Sound("Bow");  
  // Nachladen
  if(i != 2)
  {
  	SetAction("Reload");
  	pClonk->~LoadBow();
  	// Munitionscheck
  	if(!CheckMunitionID(Local(9)))
  	{
  	  // Leer? Dann andere Munition
  	  ControlDig(Contained());
  	}
  }
  return(1);
}
