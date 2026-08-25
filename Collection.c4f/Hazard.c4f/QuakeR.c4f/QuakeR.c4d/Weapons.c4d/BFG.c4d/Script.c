/*-- BFG --*/

#strict

#include QWPN

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(1500); }


public func FMData1(int data)
{
  if(data == FM_Name)      return("Boom");
  if(data == FM_AmmoID)    return(QBFS);
  if(data == FM_AmmoLoad)  return(5);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(30);

  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(1);

  if(data == FM_Damage)    return(66);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(8);

  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->WeaponEnd(x,y);

  	var a = angle;
  	var ammo = CreateObject(QSBF,x,y,GetController(user));
  //int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize
  	ammo->Launch(a,500,1000,5,20, GetFMData(FM_Damage),0,2);

  // Effekte
  MuzzleFlash2(75,user,x,y,angle,RGBa(20,200,255,0));
  Sound("bfgfire");
}

// wenn leer verschwindet sie
func OnEmpty()
{
	RemoveObject();
}

func GetAmmoPackID() { return(); }
func GetSpawnColor() { return(RGBa(100,150,255,100)); }
