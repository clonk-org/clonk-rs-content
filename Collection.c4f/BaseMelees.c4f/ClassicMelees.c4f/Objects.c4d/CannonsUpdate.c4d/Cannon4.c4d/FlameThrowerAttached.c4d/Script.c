/*--- Flammenwerfer (angeschraubt) ---*/

#strict

#include CTW2

local FillLevel;
local GasLevel;

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(ACT1); }

/* Turm weg und Munitionscheck */

public func ComEnter(object pAmmo)
{
  // Gasfasser in Munition umwandeln
  if(GetID(pAmmo) == ABRL)
  {
    Sound("Click");
    GasLevel += pAmmo->ABRL::GetAmount();
    FillLevel += pAmmo->ABRL::GetAmount();
    pAmmo->OBRL::BarrelDoFill(-pAmmo->ABRL::GetAmount());
    ChangeDef(pAmmo->EmptyBarrelID(),pAmmo);
  }

  // Ölfasser in Munition umwandeln
  if(GetID(pAmmo) == OBRL)
  {
    Sound("Click");
    FillLevel += pAmmo->OBRL::GetAmount();
    pAmmo->OBRL::BarrelDoFill(-pAmmo->OBRL::GetAmount());
    ChangeDef(pAmmo->EmptyBarrelID(),pAmmo);
  }
}

public func ComFire(object pClonk)
{
  // Rotation stoppen
  Sound("Click");
  SetAction("Attaching", GetActionTarget());
  SetRDir(0);
  
  // Säure suchen
  var ammo = FindContents(ABRL,GetActionTarget());
  if(ammo) ComEnter(ammo);

  // Öl suchen
  var ammo = FindContents(OBRL,GetActionTarget());
  if(ammo) ComEnter(ammo);
  
  // Muni aus Öl oder Säure erzeugen
  if(FillLevel < 50) return(Sound("Error") );
  var ShootGas;
  if(FillLevel/2 <= GasLevel)
	{
	GasLevel -= 50;
	ShootGas = 1;
	}
  FillLevel -= 50;

  if(ShootGas)
	{
	// Gassschuss ausführen
	var iAngle = GetR();
	if(iAngle>0) { CreateGasShoot(this(),40,Sin(iAngle,0),Cos(iAngle,0)-20,-Abs(iAngle)-90);}
	else CreateGasShoot(this(),40,Sin(iAngle,0),Cos(iAngle,0)-20,Abs(iAngle)-90);
	Sound("Fuse");
	return();
	}

  // Feuerschuss ausführen
  var iAngle = GetR();
  if(iAngle>0) { CreateFireShoot(this(),40,Sin(iAngle,0),Cos(iAngle,0)-20,-Abs(iAngle)-90);}
  else CreateFireShoot(this(),40,Sin(iAngle,0),Cos(iAngle,0)-20,Abs(iAngle)-90);
  Sound("FlameThrower");
}
