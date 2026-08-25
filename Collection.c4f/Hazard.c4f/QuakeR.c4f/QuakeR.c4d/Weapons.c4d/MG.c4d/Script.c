/*-- MG --*/

#strict

#include QWPN

// Anzeige in der Hand
public func HandSize() { return(750); }
public func HandX()    { return(7500); }
public func HandY()    { return(-2000); }
public func BarrelXOffset(){return(-2000);}
public func BarrelYOffset(){return(-2000);}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(QMGS);
  if(data == FM_AmmoLoad)  return(15);

  if(data == FM_Reload)    return(5);
  if(data == FM_Recharge)  return(5);

  if(data == FM_AmmoUsage) return(3);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(true);

  if(data == FM_Damage)    return(20);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(800);
  if(data == BOT_DmgType)  return(DMG_Projectile);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_Priority) return(0);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Scatter$");
  if(data == FM_AmmoID)    return(QMGS);
  if(data == FM_AmmoLoad)  return(15);

  if(data == FM_Reload)    return(5);
  if(data == FM_Recharge)  return(5);

  if(data == FM_AmmoUsage) return(3);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(true);

  if(data == FM_Damage)    return(13);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(200);
  if(data == BOT_DmgType)  return(DMG_Projectile);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(0);

  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10);
  var x,y; user->WeaponEnd(x,y);
  for(var i; i < 3; i++)
  {
  	var a = angle + RandomX(-3,+3);
  	var ammo = CreateObject(QSHT,x,y,GetController(user));
  //int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize
  	ammo->Launch(a,200+Random(100),800,2,10, GetFMData(FM_Damage),1,1);
  }

	Sound("MGfire*");

  // Effekte
  MuzzleFlash(30,user,x,y,angle);
  var angle2;
  if(angle > 0 && angle < 180)
  	angle2 = angle-(90+50);
  else
  	angle2 = angle+90+50;
  user->WeaponBegin(x,y);
  BulletCasing(x+Sin(angle,6),y-Cos(angle,6),+Sin(angle2+RandomX(-5,5),25),-Cos(angle2+RandomX(-5,5),25),4);
}

public func Fire2()    // Streufeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10);
  var x,y; user->WeaponEnd(x,y);
  for(var i; i < 6; i++)
  {
  	var a = angle + RandomX(-10,+10);
  	var ammo = CreateObject(QSHT,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  	ammo->Launch(a,200+Random(100),800,2,10, GetFMData(FM_Damage),1	,1);
  }

	Sound("MG2fire*");

  // Effekte
  MuzzleFlash(30,user,x,y,angle);
  var angle2;
  if(angle > 0 && angle < 180)
  	angle2 = angle-(90+50);
  else
  	angle2 = angle+90+50;
  user->WeaponBegin(x,y);
  BulletCasing(x+Sin(angle,6),y-Cos(angle,6),+Sin(angle2+RandomX(-5,5),25),-Cos(angle2+RandomX(-5,5),25),4);
}

func OnAutoStart()
{
	SetAction("Shoot");
}

func OnAutoStop()
{
	SetAction("RunOut");
}

func GetAmmoPackID() { return(QMGA); }
func GetSpawnColor() { return(RGBa(252,244,27, 100)); }
