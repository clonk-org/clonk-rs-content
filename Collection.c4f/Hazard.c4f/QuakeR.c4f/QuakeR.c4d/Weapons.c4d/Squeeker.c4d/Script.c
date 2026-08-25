/*-- Grenadelauncher --*/

#strict

#include QWPN

local counter;

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(4000); }
public func HandY()    { return(-1500); }
public func HandR()    { return(-5); }

public func FMData1(int data)
{
  if(data == FM_Name)     return("Squeeker");
  if(data == FM_AmmoID)   return(QSQS);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(35);
  if(data == FM_Recharge) return(35);

  if(data == FM_Damage)   return(1);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(100);
  if(data == BOT_DmgType)  return(DMG_Bio);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_Priority) return(4);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)     return("Squeeker-Mine");
  if(data == FM_AmmoID)   return(QSQS);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(80);
  if(data == FM_Recharge) return(80);

  if(data == FM_Damage)   return(20);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(100);
  if(data == BOT_DmgType)  return(DMG_Bio);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_Priority) return(4);

  return(Default(data));
}

public func Fire1()
{
	var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle();

	for(var i; i < GetFMData(FM_Damage); i++)
	{
  	var squeeker = CreateObject(QSQK, x, y, -1);
  	squeeker->Launch(angle+RandomX(3,-3));
  }
}

public func Fire2()
{ 
	var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle();

  var squeeker = CreateObject(QSQK, x, y, -1);
  // winkel + einbuddeln
  squeeker->Launch(angle+RandomX(3,-3), true);
}

func GetAmmoPackID() { return(QSQA); }
func GetSpawnColor() { return(RGBa(234,0,239, 100)); }
