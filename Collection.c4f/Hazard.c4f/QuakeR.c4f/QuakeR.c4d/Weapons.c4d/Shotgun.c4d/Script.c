/*-- Shotgun --*/

#strict

#include QWPN

public func HandSize() { return(800);  }
public func HandX()    { return(6000); }
public func HandY()    { return(500);  }
public func BarrelYOffset(){return(-1000);}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(QSGS);
  if(data == FM_AmmoLoad)  return(4);
  if(data == FM_AmmoUsage) return(4);

  if(data == FM_Reload)    return(32);
  if(data == FM_Recharge)  return(32);

  if(data == FM_Damage)    return(3);
  //if(data == FM_Auto)      return(true);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(200);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(7);

  return(Default(data));
}

public func Fire1()
{ 
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(15);
  var ammo;

  for(var i=0; i<12; i++)
  {
    ammo = CreateObject(QSTG, x, y, GetController(user));
    ammo->Launch(angle+RandomX(10,-10),180+Random(140),400,2,10, GetFMData(FM_Damage),0,1);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);
	
	user->WeaponBegin(x,y);

	var dir = GetDir(user)*2-1;
	BulletCasing(x, y, -dir*(Random(15)+5), -Random(15)-15, 5, RGB(255,100,0));
  BulletCasing(x, y, -dir*(Random(15)+5), -Random(15)-15, 5, RGB(255,100,0));

  // Sound
  Sound("SGfire*");
}

func GetAmmoPackID() { return(QSGA); }
func GetSpawnColor() { return(RGBa(238,151,0, 100)); }
