/*-- Ritter --*/

#strict

#include KARH

func InitType() { }

// Kann nur eine Sache, seinen Bogen tragen
public func MaxContentsCount() { return 0; }
public func RejectCollect() { return 1; }
public func GetMaxSpecialCount(iIndex, fAmount) { return 0; }

public func GetFightAction()  { return "Fight"; } /* Kampfaktion */
public func GetStrikeAction() { if(!Random(2)) return "Thrust"; return "Strike"; } /* Schlagaktion */
public func GetStrikeTime() {  return 1; } /* Kampfgeschwindigkeit */
public func GetStrikeDamage() { return 50; } /* Kampfschaden */
public func StrikeSound() { Sound("SwordHit*"); } /* Schlag */

//! Liefert den Schutzfaktor.
public func IsShielded(fArrow)
{
  // Wert zurückliefern
  return 40;
}

public func HasArmor() { return 0; }

/* Fallschaden */
protected func Hit(int xdir, int ydir)
{
	var dist = Distance(0, 0, xdir, ydir);
//	Log("Hit %d", dist);
	if(dist > 800)
	{
//		Log("HEAVY HIT------------");
	  DoEnergy(-100);
	}
  Sound("ClonkHit*");
  return _inherited(...);
} 


/*
protected func Striking()
{     
  var aRect = Find_InRect(-20,-10,20,20);
  if(GetDir() == DIR_Right) aRect = Find_InRect(0,-10,20,20);
   
  var pClonk = FindObject2(Find_OCF(OCF_Alive),Find_Hostile(GetOwner()),aRect,Find_Layer(GetObjectLayer()),Find_NoContainer());
  if(pClonk)
    Punch(pClonk,23+RandomX(0,5));
}*/