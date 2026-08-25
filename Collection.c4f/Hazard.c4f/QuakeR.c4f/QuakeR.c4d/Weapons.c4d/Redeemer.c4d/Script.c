/*-- Redeemer --*/

#strict

#include QWPN

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(1500); }


public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_3);
  if(data == BOT_Priority) return(8);

  return(Default(data));
}

public func FMData1(int data)
{
  if(data == FM_Name)   return("$Missile$");
  if(data == FM_AmmoID)   return(QRDS);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(42);
  if(data == FM_Recharge)   return(42);

  if(data == FM_Damage) return(27);

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
  if(data == FM_Name)   return("$CMissile$");
  if(data == FM_AmmoID)   return(QRDS);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(42);
  if(data == FM_Recharge)   return(42);

  if(data == FM_Damage) return(27);

  return(Default(data));
}

public func Fire1() { LaunchRocket(QRDM,Contained()->~AimAngle(10), GetFMData(FM_Damage, 1)); }
public func Fire2() { LaunchRocket(QRDM,Contained()->~AimAngle(10), GetFMData(FM_Damage, 2), 0, true); }

public func LaunchRocket(rid, angle, dmg, speed, ctd) {

  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);
  // not too near to clonk
  x = x*3/2;
  y = y*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, 70, 200, speed, ctd);

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
  Sound("RDFire");
}

// wenn leer verschwindet sie
func OnEmpty()
{
	RemoveObject();
}

func GetAmmoPackID() { return(QRDA); }
func GetSpawnColor() { return(RGBa(100,150,255,100)); }
