/*-- Plasmagun --*/

#strict

#include QWPN

local counter;
local fchargefire; //Sollen wir feuern?

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(1500); }
public func BarrelXOffset(){return(1500);}
public func BarrelYOffset(){return(-2000);}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(QPGS);
  if(data == FM_AmmoLoad)  return(15);

  if(data == FM_Reload)    return(6);
  if(data == FM_Recharge)  return(6);

  if(data == FM_AmmoUsage) return(3);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(true);

  if(data == FM_Damage)    return(10);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_Priority) return(3);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Charge$");
  if(data == FM_AmmoID)    return(QPGS);
  if(data == FM_AmmoLoad)  return(30);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(3);

  if(data == FM_AmmoUsage) return(3);
  if(data == FM_AmmoRate)  return(2);
  if(data == FM_Auto)      return(true);
  if(data == FM_Charge)   return(true);

  if(data == FM_Damage)    return(9);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(300);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_Priority) return(3);

  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10);
  var x,y; user->WeaponEnd(x,y);
  for(var i; i < 1; i++)
  {
  	var a = angle + RandomX(-4,+4);
  	var ammo = CreateObject(QSHP,x,y,GetController(user));
  //int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize
  	ammo->Launch(a,250+Random(50),600,2,10, GetFMData(FM_Damage),0,8);
  }

  // Effekte
  MuzzleFlash2(50,user,x,y,angle,RGBa(20,255,200,0));
  SetAction("Shoot");
  Sound("PGfire*");
}

public func Fire2()    // Aufladefeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10);
  var x,y; user->WeaponEnd(x,y);
  if (!CheckAmmo(QPGS,counter*2/3)) fchargefire = true;
  if(fchargefire) //Nicht mehr HAX!
  {
		if (counter>0)
		{
			for(var i; i < 2; i++)
			{
				var a = angle + RandomX(-10,+10);
				var ammo = CreateObject(QSHP,x,y,GetController(user));
				//int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize
				ammo->Launch(a,250+Random(50),800,2,10, GetFMData(FM_Damage),0,7);
				MuzzleFlash2(80,user,x,y,angle,RGBa(20,255,200,50));
			}
			counter -= 3;
			Sound("PG2fire*");
		}
		else
		{
			StopAutoFire();
			Empty();
			Reload();
		} 	
	}
  else
  {
  	//CreateParticle("PSpark",x,y,0,0,(20-ratecount)*10,
      //           RGBa(191,255,255,0),this()); 	
  	SetAction("Recharge");
  	// Phase setzen: von hinten 6 Phasen
  	var p = ratecount-8;
  	if(!p)
  		SetAction("Shoot");
  	else
  		SetPhase(p/2);
  	//Charge!
  	counter +=2;
  }
}

func OnAutoStart()
{
	counter = 0;
	fchargefire=false;
}

func OnAutoStop()
{
	// nur im FM2 muni zurückgeben
	if(GetFireMode() != 2)
		return();
	
	var amount;
	if(fchargefire)
	{
		DoAmmo(QPGS,counter*15/24);
		counter = 0;
	}
	else
	{
		DoAmmo(QPGS,GetFMData(FM_AmmoLoad,2) - GetAmmo(QPGS));
	}
}

func OnChargeFire()
{
	fchargefire=true;
}

func GetAmmoPackID() { return(QPGA); }
func GetSpawnColor() { return(RGBa(197,0,255,100)); }
