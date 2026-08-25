/*-- Grenadelauncher --*/

#strict

#include QWPN

local counter;

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(7000); }
public func HandY()    { return(-3500); }
public func HandR()    { return(-5); }
public func BarrelXOffset(){return(-2000);}
public func BarrelYOffset(){return(-2000);}

public func FMData1(int data)
{
  if(data == FM_Name)     return("$Grenade$");
  if(data == FM_AmmoID)   return(QGLS);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(45);
  if(data == FM_Recharge) return(45);

  if(data == FM_Damage)   return(25);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(2);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)     return("$FragGrenade$");
  if(data == FM_AmmoID)   return(QGLS);
  if(data == FM_AmmoLoad) return(2);
  if(data == FM_AmmoUsage)return(2);
  if(data == FM_AmmoRate) return(20);
  if(data == FM_Reload)   return(80);
  if(data == FM_Recharge) return(2);

  if(data == FM_Damage)   return(60);
  
  if(data == FM_Auto)     return(true);
  if(data == FM_Charge)   return(true);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Projectile);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(2);

  return(Default(data));
}

public func Fire1() { LaunchGrenade(QGLG, 90,Contained()->~AimAngle( 5)+RandomX( -1, 1),1); }
public func Fire2()
{ 
	counter++;
	if(counter >= GetFMData(FM_AmmoRate))
		LaunchGrenade(QGLC, 70,Contained()->~AimAngle(10)+RandomX(-2,2),2);
}

func OnAutoStart()
{
	counter = 0;
}

func OnChargeFire()
{
	LaunchGrenade(QGLC, 70,Contained()->~AimAngle(10)+RandomX(-2,2),2);
	DoAmmo(GetFMData(FM_AmmoID,2), -GetFMData(FM_AmmoUsage,2));
	StopAutoFire();
	Reload();
}

public func LaunchGrenade(id idg, int speed, int angle, int mode) {
  var user = Contained();
  var dir = GetDir(user)*2-1;

  // Adjust angle
  angle = BoundBy(angle,-360,360);
  // calculate speed
  var xdir = Sin(angle,speed);
  var ydir = -Cos(angle,speed);

  var x,y;
  user->WeaponEnd(x,y);

  // create and launch
  var grenade=CreateObject(idg, x+xdir/10, y+ydir/10, GetController(user));
  grenade->Launch(xdir+GetXDir(user)/2, ydir+GetYDir(user)/2, GetFMData(FM_Damage, mode));

	if(mode == 2)
		grenade->SetCounter(counter);

  // effect
  Sound("GLFire*");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
                   GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
                   RandomX(80,140),RGBa(200,200,200,0),0,0);
  }
}

func GetAmmoPackID() { return(QGLA); }
func GetSpawnColor() { return(RGBa(2,144,18, 100)); }
