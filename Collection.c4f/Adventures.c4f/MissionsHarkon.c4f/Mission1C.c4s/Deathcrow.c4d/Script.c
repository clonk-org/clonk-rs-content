/*-- Vogel --*/

#strict 2
#include BIRD

public func IsPossessible() { return 0; }

public func SearchRange() { return 300; }

local pTarget;
local pLastWayp;
local pComeFromWayp;

/* TimerCall mit KI-Steuerung */

protected func Activity()
{  // Nicht zu hoch fliegen
  if(GetY()<12) SetComDir(COMD_Down);
  // gehört dem Gegner
  if(GetOwner() == -1) SetOwner(10);
  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  if(pTarget && !Random(10))
	{
		//pTarget = 0;
		SetCommand(this, "None");
	}
  // Gegner suchen
  if(!pTarget)// && !GetCommand())
  {
    pTarget = FindNearestEnemy();//FindObject2(Find_OCF(OCF_CrewMember), Find_NoContainer(), aHostile, Find_Distance(SearchRange()));
  }
  
  // Beute am Maul beißen
  if (Attack()) return;
  
  // Angreifen
  if(pTarget)
  {
    // Wenn Gegner im Haus, tot oder zu weit weg abbrechen	  
    if(Contained(pTarget)) { SetCommand(this, "None"); return pTarget=0; }	 
    if(!GetAlive(pTarget)) { SetCommand(this, "None"); return pTarget=0; }
    if(ObjectDistance(pTarget)>SearchRange()) { SetCommand(this, "None"); return pTarget=0; }
    // sonst zum Gegner fliegen
    var iFromSide = +1; if (GetX(pTarget)>GetX()) iFromSide = -1;
		var speed = Distance(GetXDir(0, 100), GetYDir(0, 100));
		var angle = Angle(GetX(), GetY(), GetX(pTarget) + 13 * iFromSide, GetY(pTarget));
		var speed_max = 4000;
		if(speed < speed_max) speed += 100;
//		var angle2 = Angle(0, 0, GetXDir(), GetYDir());
//	if(Abs(angle2-angle) > 10)
		SetXDir(Sin(angle, speed), 0, 100); SetYDir(-Cos(angle, speed), 0, 100);
    SetMacroCommand("MoveTo", 0, GetX(pTarget) + 13 * iFromSide, GetY(pTarget));
    if(iFromSide>0 && GetDir()) TurnLeft();
    if(iFromSide<0 && !GetDir()) TurnRight();
    return;
  }
  
  if(!(GetCommand() && Random(20)))
	{
  var wayp1 = FindObject(WAYP, 0, 0, -1, -1);
	var wayp = wayp1->GetPathTarget(Random(wayp1->GetPathCount()));
	if(GetXDir() > 0 && GetDir() == 0) TurnRight();
	if(GetXDir() < 0 && GetDir() == 1) TurnLeft();
	if(pLastWayp != wayp && pComeFromWayp != wayp)
	{
		pLastWayp = wayp;
		pComeFromWayp = wayp1;
		var speed = Distance(GetXDir(0, 100), GetYDir(0, 100));
		var speed_max = 4000;
		if(speed < speed_max) speed += 100;
		var angle = Angle(0, 0, GetX(wayp), GetY(wayp));
		SetXDir(Sin(angle, speed), 0,  100); SetYDir(-Cos(angle, speed), 0, 100);
		return SetMacroCommand("MoveTo", wayp);
	}
	}
	
	if(GetContact(0, -1)) SetCommand(this, "None");
  if(GetContact(0, -1, CNAT_Top)) SetComDir(COMD_Down);
	if(GetContact(0, -1, CNAT_Bottom)) SetComDir(COMD_Up);
	if(GetContact(0, -1, CNAT_Left)) SetComDir(COMD_Right);
	if(GetContact(0, -1, CNAT_Right)) SetComDir(COMD_Left);
  
  // Nichts machen
  if (Random(2) || GetAction() != "Fly") return 1;

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up);
  if (Random(2)) SetComDir(COMD_Down);

  // Nicht zu hoch fliegen
  if(GetY()<12) SetComDir(COMD_Down);

  // Nichts machen
  if (!Random(4)) return 1;

  // Umdrehen
  if (Random(2)) return TurnRight();
  return TurnLeft();
}

/* Gegnersuche */

private func FindNearestEnemy()
  {
  var cont = Contained();
  if (cont) cont = Find_Container(cont); else cont = Find_NoContainer();
  var obj, iDistance;
  // Objekte prüfen:
  // Muss leben und im Freien sein, wenn die KI auch im Freien ist...
  for (var obj in FindObjects(Find_Distance(SearchRange()), cont, Find_OCF(OCF_Alive), Sort_Distance(), Find_Hostile(GetOwner())))
    if ((GetOwner()<0 && GetOwner(obj)>=0) || Hostile(GetOwner(obj), GetOwner())) // ...verfeindet...
      if (!GetVisibility(obj) && (GetDir() == (GetX(obj)>GetX()))) // ...sichtbar... (2do: Richtiger Check)
       if (GetRGBaValue(GetClrModulation(obj), 0)<( (SearchRange()-ObjectDistance(obj))*200/SearchRange()))
        if (GetOCF(obj) & OCF_CrewMember)          // ...gegnerischer CrewMember...
          {
          if (!PathFree(GetX(), GetY(), GetX(obj), GetY(obj))) // ...in Sicht...
              continue;
          // Durch Verblendung verdeckt?
          if (FindObject2(Find_ID(LGCN), Find_InRect(GetX(obj)-GetX()-120, GetY(obj)-GetY()-120, 240,240))) continue;
          return obj; // OK, den nehmen!
          }
  }

public func Attack(object pAttack)
{
  // Während des Fliegens kann alle 2 Sekunden einmal gebissen werden
  if (GetEffect("IntAttackDelay", this) || GetAction() != "Fly") return;

  // Beute suchen
  pAttack = FindObject(0, -14 + 28 * GetDir(), 0, 0, 0, OCF_Prey, 0, 0, NoContainer());
  if(pAttack != pTarget) pAttack = 0;
  
  // Einen Ballon oder ein Luftschiff suchen
  if (!pAttack) pAttack = FindObject(BALN, -20,0,40,30, 0, 0,0, NoContainer());
  if (!pAttack) pAttack = FindObject(BLMP, -30,0,60,40, 0, 0,0, NoContainer());
  if (!pAttack) return;

  if(GetAlive(pAttack))
  {
    // Schaden machen und schleudern	  
    Punch(pAttack);
    if(GetX(pAttack) < GetX(this))
      Fling(pAttack, -1, -1);
    else
      Fling(pAttack, 1, -1);
  }
  
  // Reinbeißen!
  pAttack->~Poke();
  SetAction("Attack");
  Sound("Raven*");
  AddEffect("IntAttackDelay", this, 1, 70);
  return 1;
}

protected func Death()
{
  ChangeDef(DBBR);
  return 1;
}
