/*-- Keule --*/

#strict 2

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return 1;
}

/* Kampfaktion */
public func GetFightAction()
{
  return "ClubFight";
}

/* Schlagaktion */
public func GetStrikeAction()
{
  return "ClubStrike";
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return 2;
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return 4;
}

/* Schlag */
public func StrikeSound()
{
  Sound("ClubHit*");
}

/* Aufschlag */

protected func Hit()
{
  Sound("WoodHit*");  
  return 1;
}
  
public func ControlThrow(pClonk)
{
  return Activate(pClonk);
}

public func Activate(pClonk)
{
  [$Stab$]
  if(!Contained()->~CanUse(GetID())) return;
  if(GetAction(Contained()) != "Walk") return;
//  if(pClonk->~IsNotHidden()) return;
  var aRect = Find_InRect(-30,-10,30,20); if(GetDir(pClonk)) aRect = Find_InRect(0,-10,30,20);
  var pEnemy = FindObject2(Find_OCF(OCF_CrewMember),Find_Layer(GetObjectLayer()), aRect, Find_Hostile(GetOwner(pClonk)), Find_OCF(OCF_Alive));
  if(!pEnemy) return;
  if(GetDir(pEnemy)!=GetDir(pClonk)) return;
  pClonk->~DoShow(250);
  pClonk->SetAction("Stun", pEnemy);
  return 1;
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }
