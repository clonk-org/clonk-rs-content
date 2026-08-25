/*-- Messerpaket --*/

#strict 2
local iUsedItems;

// Pfeilpack includieren für die Paketverwaltung
#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return TKNF; }
public func IsArrowPack() { return 0; }
public func MaxPackCount() { return 3; }

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return 1;
}

/* Kampfaktion */
public func GetFightAction()
{
  return "DaggerFight";
}

/* Schlagaktion */
public func GetStrikeAction()
{
  return "DaggerStrike";
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return 2;
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

/* Aufschlag */

protected func Hit()
{
  Sound("WoodHit*");  
  return 1;
}
  
public func ControlThrow()
{
  if(!Contained()->~CanUse(GetID())) return;
  if(GetAction(Contained())=="Push") return;
  if(!ActIdle()) return 1;
  if(GetAction(Contained()) != "Walk" && GetAction(Contained()) != "Ride" && GetAction(Contained()) != "Jump") return 1;
  SetAction("Reload");
  Sound("ThrowKnife");
  Contained()->~DoShow(100);
  var pObj = Contained();
  var knife = GetItem();
  knife->Departure(pObj);
  if(GetAction(Contained()) == "Walk")
    ObjectSetAction(pObj, "Throw");
  if(GetAction(Contained()) == "Ride")
    ObjectSetAction(pObj, "RideThrow");
  return 1;
}

func FindCandleHolder()
{
	var pObj = FindObject2(Find_ID(_CHO), Find_InRect(-10, -10, 20, 40));
	if(pObj && pObj->CanBeCutDown())
		return pObj;
}

public func ContextCutDown(pClonk)
{
	[Abschneiden|Image=_CHO|Condition=FindCandleHolder]
  var pObj = FindCandleHolder();
	if(pObj) pObj->CutDown();
  return 1;
}

public func Activate(pClonk)
{
  [$Stab$]
  if(!Contained()->~CanUse(GetID())) return;
  if(GetAction(Contained()) != "Walk") return;
//  if(pClonk->~IsNotHidden()) return;
  var aRect = Find_InRect(-30,-20,30,40); if(GetDir(pClonk)) aRect = Find_InRect(0,-20,30,40);
  var pEnemy = FindObject2(Find_OCF(OCF_CrewMember),Find_Container(pClonk->Contained()), Find_Layer(GetObjectLayer()), aRect, Find_Hostile(GetOwner(pClonk)), Find_OCF(OCF_Alive));
  if(!pEnemy)
	{
		var pEnemy = FindObject2(Find_OCF(OCF_CrewMember), Find_Layer(GetObjectLayer()), aRect, Find_Hostile(GetOwner(pClonk)), Find_OCF(OCF_Alive));
		if(!pEnemy || !(pEnemy->Contained())->~IsAssassinable()) return;
	}
  if(GetDir(pEnemy)!=GetDir(pClonk) && !pEnemy->Contained()) return;
  pClonk->~DoShow(250);
  pClonk->SetAction("Assassasinate", pEnemy);
  return 1;
}

/* Grafik anpassen */
public func UpdatePicture()
{
  if(MaxPackCount()-iUsedItems>1) SetGraphics(Format("%d", MaxPackCount()-iUsedItems));
  else SetGraphics("");
  return _inherited();
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return 1; }

public func IsThrowingWeapon() { return 1; }

public func GetThrowData(& iX, & iY, & iXDir, & iYDir, iMoveDir, & idID)
{
	iX = 10; if (!iMoveDir) iX = -iX;
	iY = -6;
	iXDir = 5; if(!iMoveDir) iXDir = -iXDir;
	iYDir = -2;
	idID = TKNF;
	return 1;
}
