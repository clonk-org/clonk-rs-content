/*-- Gauntlet --*/

#strict

#include QWPN

local Offset;

func Initialize()
{
	_inherited();
	SetAction("Brazzel");
	if(QEGGknifefight)
		SetHax(true);
}

public func HandX() { return(-1000 + Offset); }
public func HandSize() { return(800); }

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Punch$");
  if(data == FM_AmmoID)    return(QGTS);
  if(data == FM_AmmoLoad)  return(1);

  if(data == FM_Reload)    return(20);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(0);

  if(data == FM_Damage)    return(35);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(15);
  if(data == BOT_DmgType)  return(DMG_Melee);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(9);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Impulse$");
  if(data == FM_AmmoID)    return(QGTS);
  if(data == FM_AmmoLoad)  return(1);

  if(data == FM_Reload)    return(20);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(0);

  if(data == FM_Damage)    return(20);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(15);
  if(data == BOT_DmgType)  return(DMG_Melee);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(9);

  return(Default(data));
}

public func Fire1()
{ 
	Offset = 1000;
	r = -60;
	AddEffect("Punch",this(),300,1,this(),Q_GT);
	Sound("Gauntlet*");
}

public func Fire2()
{ 
	r = 0;
	Offset = 1000;
	AddEffect("Punch",this(),300,1,this(),Q_GT);
	Sound("Gauntlet*");
}

func Firing()
{
	var user = GetUser();
	var x,y; user->WeaponEnd(x,y);
	var dir = GetDir(user)*2-1;
	var hit;
	
	for(var obj in FindObjects(Find_OnLine(0,0,x,y), Find_Or( Find_And(Find_OCF(OCF_CrewMember), Find_Func("CheckEnemy", user)),Find_Action("Dead")), Find_Exclude(user)))
	{
		var fobjisCrew=GetOCF(obj) & OCF_CrewMember; //Echter Gegner?
		if(GetFireMode() == 1)
		{
			if(obj->GetContact(obj,-1, CNAT_Bottom))
			{
				obj->SetPosition(GetX(obj), GetY(obj)-1);
				obj->SetAction("Jump");
			}
			obj->SetXDir(GetXDir(obj) + dir*10);
			obj->SetYDir(GetYDir(obj) + -5);
			DoDmg(GetFMData(FM_Damage,1),DMG_Melee,obj);
		}
		else
		{
			obj->SetPosition(GetX(obj), GetY(obj)-2);
			obj->SetAction("Tumble");
			obj->SetXDir(dir*GetFMData(FM_Damage,2)*3);
			obj->SetYDir(-25);
			DoDmg(GetFMData(FM_Damage,2)/2, DMG_Melee, obj);
		}
		//Humuliation
		if (fobjisCrew && !GetAlive(obj))
			{
				AddMedal(GetOwner(user),QMDH,user);
	 			Sound("humiliation", 1, 0,0, GetOwner(user)+1);
	 			Sound("humiliation", 1, 0,0, GetOwner(obj)+1);
			}
		hit = true;
	}
	
	// tolle Effekte bei hit!
	if(hit)
	{
		// die Partikel werden wirklich ziemlich viel Missbraucht. Armer Muzzleflash. :(
		MuzzleFlash2(20, user, x,y,90*dir,RGB(150,150,255));
		MuzzleFlash2(15, user, x,y,100*dir,RGB(150,150,255));
		MuzzleFlash2(15, user, x,y,80*dir,RGB(150,150,255));
		Sound("gauntletHit");
	}
}

public func OnAimStart()
{
	GetUser()->~StopAiming();
}

private func FxPunchTimer(object obj, int num, int time)
{
	if(!IsHax() || GetFireMode() == 2)
	{
		if(time <= 3)
			Offset += 2000;
		else if(Offset > 0)
			Offset -= 500;
	}
	
	if(r < 0)
		r += 6;
	
	// erst hier wird getroffen
	if(time == 5)
	{
		Firing();
	}
	
	if(time > 15)
	{
		r = -45;
		return(-1);
	}
}


// omg it's hax!
local r;
func IsHax() { return(GetAction() S= "Sword"); }
func SetHax(bool set) { if(set) { r = -45; SetAction("Sword"); } else SetAction("Brazzel"); }
func HandR() { if(IsHax()) return(r); }

public func HandX() { if(!IsHax()) return(inherited()); return(10000 + Offset/2); }
public func HandY() { if(!IsHax()) return(_inherited()); return(-5000); }
public func HandSize() { if(!IsHax()) return(inherited()); return(1200); }
