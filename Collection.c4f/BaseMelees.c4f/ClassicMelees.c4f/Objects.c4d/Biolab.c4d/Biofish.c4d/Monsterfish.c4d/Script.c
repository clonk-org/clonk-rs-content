/*-- Monsterfisch --*/

#strict

local pTarget, pCharge, iRange, iAttackDist, iClonksOnly;

private func TargetRange() {return(100);}  // Reichweite Gegner finden
private func AttackRange() {return(5);}    // Reichweite Angriff
private func OnlyClonks()  {return(1);}    // Nur Clonks angreifen
private func ChargeLevel() {return(4);}    // Timercalls bis nächster Angriff

/* Initialisierung */

protected func Initialize()
{
  iRange = TargetRange();
  iAttackDist = AttackRange();
  iClonksOnly = OnlyClonks();
  pCharge = ChargeLevel();

  SetAction("Swim");
  SetComDir(COMD_Left());
  if(Random(2)) SetComDir(COMD_Right());

  return(1);
}

/* TimerCall mit KI-Steuerung */

private func Activity() 
{
  if (pCharge) pCharge--;
  if (GetAction() eq "Walk") WalkDir();

  // Ziel verlieren
  SetCommand(this(),"None");
  pTarget = 0;
  
  // Gegner (neu) wählen
  if(!pTarget || ObjectDistance(pTarget) > iRange)
	pTarget = GetTarget();
  // Lebt das Lebewesen noch?
  if(GetOCF(pTarget) & OCF_Living)
    if(!GetOCF(pTarget) & OCF_Alive())
	pTarget = GetTarget();
  // Gefunden und bereit? Dann mal los!
  if(pTarget && !pCharge)
  {
    SetCommand(this(),"Follow",pTarget);
    if(ObjectDistance(pTarget) <= iAttackDist)		// In Reichweite? Angreifen?
	if(Attack(pTarget)) pCharge=ChargeLevel();	// Und neu laden...
  }

  // Schwimmverhalten
  if (!GBackLiquid(0, -8) && GetAction() ne "Walk") return(SetComDir(COMD_Down));
  if (Random(2)) return(1);
  if (GetAction()ne"Swim") return(1);
  if (!Random(10)) return(SetComDir(COMD_Up));
  if (!Random(10)) return(SetComDir(COMD_Down));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func WalkDir()
{
  SetComDir(COMD_Left());
  if (Random(2)) SetComDir(COMD_Right());
  return(1);
}

/* Kontakte */

protected func ContactLeft()
{
  return(TurnRight());
}

protected func ContactRight()
{
  return(TurnLeft());
}

protected func ContactTop()
{
  SetComDir(COMD_Down());
  return(1);
}

protected func ContactBottom()
{
  if (GetAction() ne "Walk") SetComDir(COMD_Up());
  if (Random(10)) SetComDir(COMD_Right());
  if (Random(10)) SetComDir(COMD_Left());
  return(1);
}

/* Aktionen */

private func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right());
  return(1);
}

private func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left());
  return(1);
}

public func Entrance(container) 
{
  // Damit der Fisch nicht aus U-Booten flieht und so.
  SetCommand(this(), "None");
}


/* Angriff */

private func GetTarget() // Für gegnersiche Clonks
{
  var obj;
  var pOCF = OCF_Alive;
  if(!iClonksOnly)
    if(!Random(3)) pOCF = OCF_Entrance;								// Objekte mit Eingangsbereich
  if(GetAction() ne "Swim") return(); 								// Nur in Flüssigkeit
  if(!Contained())  										// Nicht wenn eingesammelt  var obj;
  while (obj = FindObject(0, 0,0,-1,-1, pOCF, 0,0, NoContainer(), obj))				// Muss leben und im Freien sein, wenn die KI auch im Freien ist...
   if(GetOCF(obj) & OCF_InLiquid())								// ...im Wasser sein...
    if (ObjectDistance(obj) > iRange) break;    						// ...nicht zu weit weg sein...
      else if ((GetOwner()<0 && GetOwner(obj)>=0) || Hostile(GetOwner(obj), GetOwner()))	// ...verfeindet...
        if (!GetVisibility(obj)) 								// ...sichtbar...
            return(obj);									// OK, den nehmen!
  return(0);
}

private func Attack(object pObj)
{
  Sound("Munch*");
  DoEnergy(-4,pObj);
  if(GetOCF(pObj)&OCF_CrewMember())
  Sound("Hurt*",0,pObj);
  if(!Random(3)) SetComDir(COMD_Stop(),pObj);
  return(1);
}

/* Einwirkungen */


protected func Death()
{
  Sound("Splash*");
  CastPXS("Acid", 60, 20);
  RemoveObject();
  return(1);
}

// Einsammelbar?
protected func RejectEntrance(pNewContainer)
  {
  // Nur Eigene
  if ( GetOwner() == -1 ) return(1);
  if ( Hostile( GetOwner(pNewContainer),GetOwner(this()) ) ) return(1);
  return(0);
  }

protected func RejectCollect(id idObj, object pObj)
  {
  // Keine
  return(1);
  }

public func IsRefineryProduct() { return(1); }