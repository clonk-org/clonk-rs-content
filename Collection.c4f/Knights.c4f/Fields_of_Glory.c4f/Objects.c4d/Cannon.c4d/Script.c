/*-- Kanone --*/

#strict
#include CANN

local iRestoredAngle;
local pAmmos;


protected func DoAutoReload() { return(0); }		// Automatisch Nachladen? (Zeit in Frames)
protected func LoadsMax() { return(5); }		// Wie viele Ladungen zulassen?
protected func CanonballsOnly() { return(0); }		// Nur Kanonenkugeln laden?
protected func GetAutoAmmo()				// Kanonen-, Chemie-, Kristal-. Holz- oder Granitkugel für DoAutoReload
{
pAmmos = [_CBL,_CCL,_XCB,_GRT,_CBT];
return (pAmmos[Random(GetLength(pAmmos))]);
}

protected func CheckProjectile(id idObj)		// Prüfung zulässiger Projektile für CanonballsOnly
{
pAmmos = [_CBL,_CCL,_XCB,_GRT,_CBT];			// Kanonen-, Chemie-, Kristal-, Holz- und Granitkugeln sind erlaubt
	for (var item in pAmmos)
		if(idObj==item) return(1);
return(0);
}

protected func Initialize()
{
  SetAction("Stand");
  SetDir(Random(2));
  SetPhase(2);
}


public func ControlDig(object pClonk)
{
  // Winkel verstellen
  if(GetAction() eq "Stand")
    {
    if(GetPhase() < 5)
      {
      SetPhase(GetPhase()+1);
      Sound("CatapultSet");
      }
    return(1);
    }
}

public func ControlDownSingle(pByObject)
{
  // Steuerung an ziehende Pferde weiterleiten
  var pHorse;
  while (pHorse = FindPullingHorse(pHorse))
    {
    pHorse->~ControlDownSingle(pByObject);
    }
  return(1);
}

public func ControlThrow(pClonk)    // Feuern / Inhalt
{
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var pThing;
  if (pThing = pClonk->Contents())
    if (AllowLoad(GetID(pThing)))
      return(0);
  // Angekoppelt
  if(GetAction() ne "Stand") return();
  // Abfeuern wenn möglich, sonst Objekt herausnehmen
  return(Fire());
}

/* Maussteuerung */

public func AimToAngle(int iAngle)
{
  // Winkel anpassen
  while(iAngle > 180) iAngle-=360;
  // Richtung
  if(iAngle > 0) SetDir(DIR_Right());
  if(iAngle < 0) SetDir(DIR_Left());
  // Zielrichtung
  SetPhase(BoundBy( 12*Abs(iAngle)/180-2, 0,5));
}

/* Aktionen */

public func TurnLeft()
{
  if(GetDir() == DIR_Left()) return(1);
  if(WildcardMatch(GetAction(), "*Drive*"))
    SetAction("DriveTurn");

  SetDir(DIR_Left());
}

public func TurnRight()
{
  if(GetDir() == DIR_Right()) return(1);
  if(WildcardMatch(GetAction(), "*Drive*"))
    SetAction("DriveTurn");

  SetDir(DIR_Right());
}

protected func RestoreAngle()
{
  if(!iRestoredAngle) return(SetPhase(2));
  SetPhase(iRestoredAngle-1);
  iRestoredAngle=0;
}

/* Laden und Feuern */

private func AllowLoad(id idObj)
{
  // Nur Kanonenkugeln laden?
  while (CanonballsOnly())
	{
	if (idObj == GUNP) break;
	if (CheckProjectile(idObj)) break;
	return(0);
	}
  // Noch jede Menge Platz
  if (ContentsCount() < LoadsMax()*2-1) return(1);
  // Niemals mehr als x Objekte laden: Wird gebraucht, wenn aus ControlThrow aufgerufen!
  if (ContentsCount() >= LoadsMax()*2) return(0);
  // Nicht mehr als x-1 Schießpulver laden (Idiotenschutz)
  if (idObj == GUNP)
  {
    if (ContentsCount(GUNP) == LoadsMax()*2-1)
      return(0);
  }
  // Nicht mehr als x-1 Projektile laden, wenn noch Platz für Pulver gebraucht wird
  else
  {
    if (ContentsCount(GUNP) == 0)
      return(0);
  }
  // Laden o.k.
  return(1);
}

public func Fire(bool fAuto)
{
  // Schießpulver und Projektil suchen
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  // Schießpulver oder Projektil fehlt
  if (!pGunpowder || !pProjectile) 
  {
    Sound("Click");
    return(0);
  }
  // Schießpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var iX = 20*(GetDir()*2-1);
  var iY = GetPhase()-7;
  var iAngle = BoundBy(GetPhase()*12+45,45,120);
  var iXDir = Sin(iAngle,15*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-10);

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir,iYDir,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch & Feuer
  for (var i = 0; i < 6; ++i)
    {
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
    CreateParticle("Fire2", iX+RandomX(-5,+5), iY+RandomX(-5,+5), iXDir, iYDir, RandomX(5,12)*5, RGBa(255,255,255,50));
    }
  /*
  // Rückstoß
  var iDir = GetDir();
  SetXDir(-iXDir*2);
  SetDir(iDir);
  */
  // Erfolgreich geschossen
  if (!DoAutoReload()) return(1);
  // Wenn aktiv mit Verzögerung y nachladen
  Schedule("Reload()", DoAutoReload());
  return(1);
}

public func Reload()
{
	// Kanone ausleeren
	for (var pObj in FindObjects(	Find_Category(C4D_Object),
					Find_Container(this()),
					Find_Not(Find_ID(GUNP)),
					Find_Not(Find_Func("IsCanonball"))
      				    )
	)
	Exit(pObj);
  	// Schießpulver und Projektil suchen und ersetzen
	var pGunpowder=FindContents(GUNP), pProjectile=(FindOtherContents(GUNP));
	Message("{{LOAD}}", this());
	CreateParticle("PSpark",0,0,0,0,200,RGB(50,50,255),this(),1);
	if (!pGunpowder) 
  	{
   	  CreateContents(GUNP);
	  Sound("Click");
  	}
	if (!pProjectile) 
	{
	CreateContents(GetAutoAmmo());
	Sound("Click");
	}
	return(1);
}

/* Status */

public func IsHorseWagon() { return(1); }   // Stimmt zwar nicht, aber muss sein, damit anspannbar

private func FindPullingHorse(pHorse)
{
  while (pHorse = FindObject(0, -100,-100,200,200, OCF_Grab(), 0,0, NoContainer(), pHorse))
    if (GetAction(pHorse) eq "Pull" || GetAction(pHorse) eq "Pull2" || GetAction(pHorse) eq "Pull3" || GetAction(pHorse) eq "Turn")
      if (GetActionTarget(0,pHorse) == this())
        return(pHorse); 
  return(0);
}

/* Verbindung */

public func Connect(pHorse)
{
  CreateObject(CHBM,0,0,GetOwner())->CHBM::Connect(pHorse,this());
  Schedule("SetAction(\"Drive0\");", 1);
  Sound("Connect");
  return(1);
}

public func Disconnect(pFromHorse)
{         
  var pBeam = FindObject(CHBM,0,0,0,0,0,"Connect",pFromHorse);
  if (pBeam) RemoveObject(pBeam);
  SetAction("Stand");
  Sound("Connect");
  return(1);
}

public func Driving()
{
  // Kein Pferd
  if(!FindPullingHorse())
    return(SetAction("Stand"));
  // Bewegung
  if(GetXDir() != 0 && GetAction() ne "Drive1")
    return(SetAction("Drive1"));
  if(GetXDir() == 0 && GetAction() ne "Drive0")
    return(SetAction("Drive0"));
}

// Eigentlich sollen nur Pferde die Kanone ziehen
public func PullCheck()
{
  // Ein Pferd
  if(FindPullingHorse()) return();
  // Bewegung und ein schiebender Clonk
  if(GetXDir() != 0)
    if(FindObject(0, 0,0,0,0, OCF_CrewMember(), "Push", this(), NoContainer()))
      SetXDir();
}