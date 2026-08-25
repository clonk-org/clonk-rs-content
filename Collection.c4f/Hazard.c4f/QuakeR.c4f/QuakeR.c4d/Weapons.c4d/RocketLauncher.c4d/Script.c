/*-- RocketLauncher --*/

#strict

#include QWPN

local missleload;

// Anzeige in der Hand
public func HandSize() { return(700); }
public func HandX()    { return(10000); }
public func HandY()    { return(-2000);  }

public func FMData1(int data)
{
  if(data == FM_Name)   return("$Missile$");
  if(data == FM_AmmoID)   return(QRLS);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(42);
  if(data == FM_Recharge)   return(42);

  if(data == FM_Damage) return(19);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(6);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)   return("$4Missiles$");
  if(data == FM_AmmoID)   return(QRLS);
  if(data == FM_AmmoLoad) return(4);
  if(data == FM_AmmoUsage) return(1);
  
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Reload)   return(65);
  if(data == FM_Recharge)   return(35);
  if(data == FM_Auto)      return(true);

  if(data == FM_Damage) return(17);
  
  if(data == FM_Charge) return(true);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(6);

  return(Default(data));
}

public func Fire1() { LaunchRocket(QRLM,Contained()->~AimAngle(10), GetFMData(FM_Damage, 1)); }
public func Fire2()
{
	missleload++;
	Sound("LoadD");
	if(GetAmmo(GetFMData(FM_AmmoID)) == 1)
		FireRockets();
}

public func FireRockets()
{
	for(;missleload; missleload--)
		LaunchRocket(QRLM,Contained()->~AimAngle(10)+RandomX(-1,1), GetFMData(FM_Damage, 2), 20+20*missleload);
}

public func OnChargeFire()
{
	StopAutoFire();
	FireRockets();
	Reload();
}

public func LaunchRocket(rid, angle, dmg, speed) {

  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);
  // not too near to clonk
  x = x*3/2;
  y = y*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, dmg, speed);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
  }

  // Sound
  Sound("RLFire*");
}

func OnAutoStart()
{
	missleload = 0;
}

func OnAutoStop()
{
	if(missleload)
	{
		DoAmmo(GetFMData(FM_AmmoID,2),missleload,GetUser());
		missleload = 0;
	}
}

func GetAmmoPackID() { return(QRLA); }
func GetSpawnColor() { return(RGBa(255,1,1,100)); }
