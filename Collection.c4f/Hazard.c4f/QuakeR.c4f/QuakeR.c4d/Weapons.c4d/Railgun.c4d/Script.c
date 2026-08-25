	/*-- Railgun --*/

#strict

#include QWPN

static const Q_RL_ImpressiveHits=3; //Anzahl der benötigten Hits
static const Q_RL_ImpressiveFrames=200; //Maximaler Frameunterschied zwischen 2 Schüssen

local laser; //der Laserstrahl
local hitted;
local iHits; //Anzahl getroffene
local fLastShootHit; //War letzter Schuss erfolgreich?
local iLastShootFrame;

// Anzeige in der Hand
public func HandX() { return(4000); }
public func HandY() { return(500); }

func Initialize()
{
	hitted = CreateObject(L_OS);
	_inherited();
}

//RAILGUN!!!!!
func FMData1(int data) {
  if(data == FM_Name)       return("$Railgun$");

  if(data == FM_AmmoID)     return(QRGS);
  if(data == FM_AmmoLoad)   return(1);
  if(data == FM_AmmoUsage)  return(1);

  if(data == FM_Reload)     return(38);
  if(data == FM_Recharge)   return(38);

  if(data == FM_Damage)     return(33);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(1000);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_3);
	if(data == BOT_Priority) return(5);

  return(Default(data));
}

// XRay
func FMData2(int data) {
  if(data == FM_Name)       return("X-Ray");

  if(data == FM_AmmoID)     return(QRGS);
  if(data == FM_AmmoLoad)   return(1);
  if(data == FM_AmmoUsage)  return(1);

  if(data == FM_Reload)     return(76);
  if(data == FM_Recharge)   return(76);

  if(data == FM_Damage)     return(27);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(1000);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_Priority) return(5);

  return(Default(data));
}

//Railgun ist nur Lasergehaxxe.
public func Fire1() {
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(5);
  if(FrameCounter()-iLastShootFrame>Q_RL_ImpressiveFrames) iHits=0;
  if(!fLastShootHit) iHits=0;
  fLastShootHit=false;
  iLastShootFrame=FrameCounter();
  laser = CreateObject(QRGR,x,y,GetController(user));
  laser->SetClrModulation(user->GetColorDw());
  laser->Set(angle,2,1000,10,this());

  MuzzleFlash2(100,user,x,y,angle,user->GetColorDw());
  Sound("RGfire*");
  SetAction("Shoot");
  
  hitted->Clear();
}

// Schockwelle :D
public func Fire2() {
var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(5);
  if(FrameCounter()-iLastShootFrame>Q_RL_ImpressiveFrames) iHits=0;
  if(!fLastShootHit) iHits=0;
  fLastShootHit=0;
  iLastShootFrame=FrameCounter();
  laser = CreateObject(QRR2,x,y,GetController(user));
  laser->SetClrModulation(user->GetColorDw());
  laser->Set(angle,2,1000,10,this());

  MuzzleFlash2(100,user,x,y,angle,user->GetColorDw());
  Sound("RGfire*");
  SetAction("Shoot");
  
  hitted->Clear();
}

public func LaserStrike(object pObj, int iTime) {
  if(iTime < 1 && !hitted->ContainsItem(pObj))
  {
  	if(GetAlive(pObj))
  	{
  		if(!fLastShootHit) iHits++;
  		fLastShootHit=true;
  	}
  
  	DoDmg(GetFMData(FM_Damage), DMG_Energy, pObj);
  	hitted->AddItem(pObj);
  	
  	if(iHits>=Q_RL_ImpressiveHits)
  	{
  		GetUser()->AddMedal(GetOwner(GetUser()),QMDI,GetUser());
  		iHits=0;
  	}
  }

  return(0);
}

public func IsFiring() { return(laser); }

public func OnDeselect() { iHits=0; }

func GetAmmoPackID() { return(QRGA); }
func GetSpawnColor() { return(RGBa(0,200,0, 100)); }

// Bla.
global func MuzzleFlash2(int iSize, object pClonk, int iX, int iY, int iAngle, int iColor)
{
  //Doofes Westernpack, klaut uns einfach den Partikelnamen.
  CreateParticle("MuzzleFlash3",iX,iY,
                 +Sin(iAngle,500),
                 -Cos(iAngle,500),
                 iSize*5,iColor,pClonk);
  if(!IsDark())
    return();
  
  if(!iColor)
    iColor = RGB(255,255,255);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iColor = RGBa(r,g,b,Min(a+65,255));
  
  AddLightFlash(iSize*25, iX, iY, iColor);
}
