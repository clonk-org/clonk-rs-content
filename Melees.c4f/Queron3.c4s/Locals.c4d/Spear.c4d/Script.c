/*-- Speer --*/

#strict 2

local fThrown;

/* Status */
public func IsSpear() 
{ 
  return 1; 
}

public func CarryLimit()
{
  // Nur eines tragbar
  return 1;
}

public func Entrance()
{
  // Damit es die richtige Austrittsrotation annimmt
  SetRDir();
  fThrown = 0;
}

/* Werfen */
public func ControlThrow()
{
  // Träger ermitteln
  var pClonk = Contained();
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return 0;
  // Träger bereit zum Werfen?
  if (GetAction(pClonk) != "Walk" && GetAction(pClonk) != "Jump" && GetAction(pClonk) != "Ride" && GetAction(pClonk) != "RideStill") return;
  // Beim Träger Wurfaktion setzen (reitend)
  if (Contained()->~IsRiding())
    return ObjectSetAction(Contained(), "RideThrowSpear");
  // Beim Träger Wurfaktion setzen (normal)
  if (!ObjectSetAction(Contained(), "ThrowSpear")) return 0;
  // Fertig
  return 1;
}

public func Departure(object pThrower)
{
	if (!(GetOCF(pThrower)&OCF_CrewMember)) return;
	if (pThrower->~CanUse(GetID()) || fThrown) return;
	// Nochmal rein und das ganze doch richtig machen
	Enter(pThrower);
	fThrown = 1;
	
  var iX, iY, iR, iXDir, iYDir, iRDir; 
	// Speerwurf 
  iX = 10; if (!GetDir(pThrower)) iX = -iX; 
  iY = -6; 
  iR = 150; if(!GetDir(pThrower)) iR = 180 - iR; 
  iXDir = 5; if(!GetDir(pThrower)) iXDir = -iXDir; 
  iYDir = -2; 
  iRDir = 4; if(!GetDir(pThrower)) iRDir = -iRDir; 
  // Reitet? Eigengeschwindigkeit addieren 
  if (GetActionTarget()) if (Call("IsRiding")) 
  {
    iXDir += GetXDir(GetActionTarget()) / 10;
    // Steigungen beim Gehen werden leider nicht in die Y-Dir eingerechnet. Bei Steigungen
    // kann es daher vorkommen, dass der Wurf den Reiter vom Pferd haut. Was dagegen tun..?
    iYDir += GetYDir(GetActionTarget()) / 10; 
  }
  Exit(, iX, iY, iR, iXDir, iYDir, iRDir);
}

/* Einsetzen */

public func Activate(pClonk)
{
  [$TxtSpearAttack$|Condition=CanUse]
  if(pClonk->~CanUse(GetID(this)))
  if(pClonk->~StartSpearAttack(this)) return true;
  return false;
}

// An Clonk weiterleiten
public func CanUse(pClonk) { return pClonk->~CanUseSpearAttack(); }

/* Ist eine Handwaffe */
public func IsFightWeapon(pClonk)
{
  //if(pClonk->~CanUse(GetID(this)))return 1;
  if(GetID(pClonk) == KAND)return 1;
  return;
}

/* Kampfaktion */
public func GetFightAction()
{
  return "SpearFight";
}

/* Schlagaktion */
public func GetStrikeAction()
{
    return "SpearThrust";
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return 3;
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return 10;
}

/* Schlag */
public func StrikeSound()
{
  Sound("SwordHit*");
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }
