/*-- Ritter --*/

#strict

#include KNIG

public func GetFightAction()  { return "Fight"; } /* Kampfaktion */
public func GetStrikeAction() { return "Strike"; } /* Schlagaktion */
public func GetStrikeTime() {  return 5; } /* Kampfgeschwindigkeit */
public func GetStrikeDamage() { return 5; } /* Kampfschaden */
public func StrikeSound() { Sound("Punch*"); } /* Schlag */

// Contextfunktionen abschalten
public func AtEnergySite() { }
public func AtConstructionSite() { }
public func AtTreeToChop() { }
public func HasConstructMenu() { }

// Kann nur eine Sache, seinen Bogen tragen
public func MaxContentsCount() { return(1); }

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Hier könnten Spezialbehandlungen von Itemgruppen definiert werden
  // wie z.B. zu dem Inventar noch 30 Pfeile aufnehmen (siehe auch Ritter)
  if(iIndex == 0) { if(fAmount) return(30); return("IsArrow"); }
}

func InitType()
{
	CreateContents(ARWP); CreateContents(ARWP); CreateContents(ARWP);
  CreateContents(BOW1);
}


public func ControlDigSingle() { _inherited(); return 1; }

public func Initialize()
{
	InitType();
/*	CreateContents(ARWP); CreateContents(ARWP); CreateContents(ARWP);
  CreateContents(BOW1);*/
//  SetGraphics("Legs", this, KARH, 1, GFXOV_MODE_ExtraGraphics);
//  SetClrModulation(HSL(Random(256),128+RandomX(-30,30),128+RandomX(-30,30)), this, 1);
	return _inherited();
}

/* Schlag mit Handwaffe */
private func Striking()
{
  // Sound
  StrikeSound();
  // Kein Schaden
  if (!Random(2)) return(1);
  // Schaden ausrechnen
  var pEnemy = GetActionTarget();
  var iDamage = GetStrikeDamage();
  iDamage = iDamage * (100 - pEnemy->~IsShielded()) / 100;
  // Schlagen!
  Punch(pEnemy, iDamage);
  return 1;
}

/* Während des Kämpfens */
private func Fighting(pEnemy)
{
  // Je nach Waffengeschwindigkeit Schlag durchführen
  if (!Random(GetStrikeTime()))
    return SetAction(GetStrikeAction());
  // Weiterkämpfen
  return 1;
}
