/*-- Monster --*/

#strict
#include ANIM

public func IsPossessible() { return(1); }

/* Initialisierung */

protected func Initialize() { return(Birth()); }

func FxInRockTimer(pTarget, iNumber)
{
	if(EffectVar(0, pTarget, iNumber))
	{
		EffectVar(1, pTarget, iNumber)++;
		SetClrModulation(RGB(100-EffectVar(1, pTarget, iNumber),100,100-EffectVar(1, pTarget, iNumber)));
		if(EffectVar(1, pTarget, iNumber) >= 80)
		{
			SetAction("Walk");
			Sound("Growl*");
			SetAlive(1);
			DoEnergy(250);
			return -1;
		}
	}
	else
		EffectVar(0, pTarget, iNumber) = FindNearestEnemy();
}

/* TimerCall mit KI-Steuerung */

private func Activity()
{
	if(ActIdle())
	{
		return;
	}
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

	var pObj = FindNearestEnemy();
	if(pObj)
	{
		if( (GetX() < GetX(pObj) ) != GetDir() )
		{
			if(GetDir()) TurnLeft();
			else TurnRight();
		}
	  // Springen
		if (GetAction() eq "Walk")
			if (!Random(3)) return(DoJump());
		return;
	}
	
  // Nichts machen
  if (Random(2) || (GetAction() ne "Walk" && GetAction() ne "Swim")) return(1);

  // Springen
  if (GetAction() eq "Walk")
   if (!Random(3)) return(DoJump());

  // Umsehen
  if (GetAction() eq "Walk")
   if (!Random(8)) return(SetAction("LookUp"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

func SearchRange() { return 100; }

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
//       if (GetRGBaValue(GetClrModulation(obj), 0)<( (SearchRange()-ObjectDistance(obj))*200/SearchRange()))
        if (GetOCF(obj) & OCF_CrewMember)          // ...gegnerischer CrewMember...
          {
          if (!PathFree(GetX(), GetY(), GetX(obj), GetY(obj))) // ...in Sicht...
              continue;
          // Durch Verblendung verdeckt?
          if (FindObject2(Find_ID(LGCN), Find_InRect(GetX(obj)-GetX()-120, GetY(obj)-GetY()-120, 240,240))) continue;
          return obj; // OK, den nehmen!
          }
  }

/* Kontakte */

protected func ContactLeft()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  
  return(TurnRight());
}

protected func ContactRight()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  
  return(TurnLeft());
}

/* Aktionen */

public func TurnRight()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right());
  SetComDir(COMD_Right()); 
  return(1);
}

public func TurnLeft()
{
  if (Stuck() || (GetAction() ne "Walk" && GetAction() ne "Swim")) return();
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left());
  SetComDir(COMD_Left()); 
  return(1);
}

private func HitCheck()
{
  var obj;
  if (obj = FindObject(0, +1,0,0,0, OCF_Prey(), 0,0, NoContainer()))
   Punch(obj, 10);
  return(1);
}

public func DoJump()
{
  if (GetAction() ne "Walk") return();
  if (Random(2)) Sound("Growl*");
  Jump();
}

/* Einwirkungen */

protected func Death()
{
	SetClrModulation();
	SetObjectBlitMode();
  Sound("DeathGrowl");
  SetDir(DIR_Left());
  ChangeDef(DMNS);
  SetAction("Dead");
	CreateObject(GHST)->Activate(131);
  return(1);
}

/* Vermehrung */

private func ReproductionRate() { return(2000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxAnimalCount()   { return(4); }    // Maximale Tieranzahl im Umkreis

private func CountMe()
{
  var ReprodSize = ReproductionAreaSize();
  var ReprodSizeHalb = ReprodSize  / -2;
  return(ObjectCount(MONS, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , OCF_Alive()) + ObjectCount(MEGG, ReprodSizeHalb, ReprodSizeHalb, ReprodSize , ReprodSize , 0));
}

private func SpecialRepr()
{
  Exit(CreateContents(MEGG), 0, 10);
  return(1);
}

/* Steuerung durch Besessenheit */

protected func ControlCommand(szCommand, pTarget, iTx, iTy)
{
 // Bewegungskommando
 if (szCommand eq "MoveTo")
  return(SetCommand(this(),szCommand, pTarget, iTx, iTy));
 return(0);
}

protected func ContainedLeft(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  TurnLeft();
 return(1);
}

protected func ContainedRight(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
  TurnRight();
 return(1);
}

protected func ContainedUp(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");
 if(Contained()) return SetCommand(this, "Exit");
 if (GetAction() eq "Swim")
 {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Up());

  return(1);
 }

 DoJump();
 return(1);
}

protected func ContainedDown(object caller)
{
 [$TxtMovement$]
 SetCommand(this(), "None");

 if (GetAction() eq "Swim")
 {
  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Down());
  return(1);
 }

 if (GetAction() eq "Walk")
  SetAction("LookUp");
  
 return(1);
}

/* JumpAndRun Steuerung */

private func ClearDir(bool fX)
{
  if(fX && GetXDir())
  {
    if(GetXDir() > 0) SetXDir(Max(GetXDir() - 2, 0));
    else SetXDir(Min(GetXDir() + 2, 0));
  }
  if(!fX && GetYDir())
  {
    if(GetYDir() > 0) SetYDir(Max(GetYDir() - 2, 0));
    else SetYDir(Min(GetYDir() + 2, 0));
  }
}

public func ContainedUpdate(object self, int comdir)
{
  if(GetAction() S= "Swim")
  {
    SetComDir(comdir);
    ClearScheduleCall(this(), "ClearDir");
    if(comdir == COMD_Down || comdir == COMD_Up) ScheduleCall(this(), "ClearDir", 1, (Abs(GetXDir())+1)/2, true);
    if(comdir == COMD_Left || comdir == COMD_Right) ScheduleCall(this(), "ClearDir", 1, (Abs(GetYDir())+1)/2, false);
  }
  else
  {
    if(comdir == COMD_UpRight || comdir == COMD_DownRight) comdir = COMD_Right;
    if(comdir == COMD_Up || comdir == COMD_Down) comdir = COMD_Stop;
    if(comdir == COMD_UpLeft || comdir == COMD_DownLeft) comdir = COMD_Left;
    SetComDir(comdir);
  }

  return(1);
}

protected func ContainedThrow() { return(1); }

protected func ContainedDigDouble()
{
 [$TxtLeave$]
 RemoveEffect("PossessionSpell", this());
 return(1);
}
