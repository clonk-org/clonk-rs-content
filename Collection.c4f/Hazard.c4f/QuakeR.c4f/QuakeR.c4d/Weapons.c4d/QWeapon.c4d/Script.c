/*-- Quake-Waffe --*/

#strict
#include WEPN

static const FM_Charge = 99;  // Ist ein Auflademodus
static const BOT_Priority = 199; // Priorität der Waffe für den Bot


// wollen wir nicht! :I
global func HelpMessage() {}

func Initialize()
{
	_inherited();
	DoAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoLoad));
	SetGraphics("Ammo",this(),QWPN,1,GFXOV_MODE_Picture,0,0);
	ScheduleCall(this(),"AdjustOverlay",1);
}


// Quake-Waffen kann man nicht fallenlassen
func Departure(object left)
{
	if(!GetAlive(left))
	{
		if(left->~IsClonk())
			return(RemoveObject());
		else
			return();
	}
	Enter(left);
	ShiftContents(left, 0, GetID());
}

public func ControlDig(caller)
{
	if(GetFMData(FM_Name, 2)) 
		return(ControlThrow(caller, true));
}

public func ControlThrow(caller, bool altfire)
{
	// Aufladeschuss?
	if(GetFMData(FM_Charge))
	{
		if(IsShooting())
		{
			OnChargeFire();
			return(true);
		}
	}

	// Beim Nachladen/Rechargen können wir nix tun :(
	if(IsReloading() || IsRecharging() || IsShooting())
		return(true);

	SetUser(caller);
	
	Empty();
	
	// Richtigen Feuermodus auswählen
	if(altfire)
		SetFireMode(2);
	else
		SetFireMode(1);
	
	AdjustOverlay(GetUser());
	
	if(CheckAmmo(GetFMData(FM_AmmoID),GetFMData(FM_AmmoUsage),GetUser()))
		Reloaded(GetUser());
	else
	{
		Sound("Klack",0,this,100,GetController(caller)+1);
		return(true);
	}

	return(inherited(caller));
}

private func Recharge(bool onselect)    // Wartezeit zwischen zwei SchÃ¼ssen
{
  var rechargetime;
	rechargetime = GetFMData(FM_Recharge);
	if(onselect)
		rechargetime = (rechargetime*3)/4;
  var effect;
  effect = AddEffect("Recharge", this(), 1, 1+Max(1, rechargetime), this(),0);
  if(onselect)
  	EffectVar(0,this(), effect) = true;
}

public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
	if(!EffectVar(0,pTarget,iNumber))
		return(inherited(pTarget,iNumber,iReason,fTemp));
}

public func GetRecharge() {
  if(!IsRecharging())
    return(100);

  var time = GetEffect("Recharge",this(),0,6);
  // anwahl-recharge?
  if(EffectVar(0,this(),GetEffect("Recharge",this(),0)))
  	return((100*time)/(GetFMData(FM_Recharge)*3/4));
  
  return((100*time)/GetFMData(FM_Recharge));
}

public func Reload()    // Waffe nachladen
{
	AdjustOverlay();
	
  ResumeReload();
  if(IsReloading()) return(false);

  // Verzögerung? Abbrechen
  if(IsRecharging()) RemoveEffect("Recharge", this());

  // nicht genug Ammo um nachzuladen
  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  if(!CheckAmmo(ammoid,ammousage,GetUser()) && !NoAmmo())
    return(false);

  // entleeren falls noch voll
  if(CheckAmmo(ammoid,ammousage,this()))
    Empty();

  if(GetFMData(FM_Auto))
    OnAutoStop(firemode);
  OnReload(firemode);
  var reloadtime = GetFMData(FM_Reload)*MaxReloadAmount(GetUser())/GetFMData(FM_AmmoLoad);

  // Mit dem Nachladen beginnen
  AddEffect("Reload", this(), 1, 1, this(), 0, Max(1, reloadtime));
  return(true);
}

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  SetUser(caller);
  if(IsRecharging() && !throw && !GetFMData(FM_Auto))
    StopAutoFire();
}

// Keine Feuermodi
public func ControlDigDouble()
{
	return();
}

public func OnSelect()
{
	if(IsReloading())
		return();
	if(IsShooting())
		return();
	// sicherheitshalber.
	shooting = false;
	stopauto = true;
	Empty();
	SetFireMode(1);
	if(CheckAmmo(GetFMData(FM_AmmoID),GetFMData(FM_AmmoUsage),GetUser()))
	{
		Reloaded(GetUser());
		// custom recharge
		Recharge(true);
	}
	else
		return(true);
}

public func OnDeselect()
{
	Empty();
}


public func OnEmpty()
{
	if(!IsReloading())
		Reload();
}

public func OnChargeFire() {} // Dummy

public func PauseReload() // Nachladen pausieren brauchen wir nicht
{}

private func Reloaded(caller)
{
	AdjustOverlay(caller);
	_inherited(caller);
	AdjustOverlay(caller);
}


func Entrance(object byobj)
{
	if(byobj != GetUser())
		ScheduleCall(this(), "DoCollectSound", 1,0,byobj);

	_inherited(byobj);
	AdjustOverlay(byobj);
}

func DoCollectSound(object byobj)
{
	Sound("collectWeapon",0,byobj,0,GetController(byobj)+1);
}

private func AdjustOverlay(caller)
{
	if(!caller)
		caller = Contained();
	
	var ammoid = GetFMData(FM_AmmoID);
	var maxammo = ammoid->MaxAmmo();
	var atmammo = GetAmmo(ammoid,this()) + GetAmmo(ammoid,caller);
	
	var height = Min(1000,(1000*atmammo)/(maxammo));
	
	if(NoAmmo())
		height = 1000;
	
	SetObjDrawTransform(1000,0,0,0,1000-height,-height*18,this(),1);
}

func OnAmmoChange()
{
	AdjustOverlay();
}

public func FxReloadStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // nicht nachladen
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  // temporäre Aufrufe ignorieren
  if(fTemp) return();
  Message(" ",pTarget);
  // nur erfolgreich nachgeladen wenn Timer abgelaufen
  if(iReason == 0)
  {
  	if(pTarget->NoAmmo())
  		pTarget->DoAmmo(pTarget->GetFMData(FM_AmmoID), pTarget->GetFMData(FM_AmmoLoad), pTarget->GetUser());
    pTarget->Reloaded(GetUser());
  }
}


// Nach Hazardupdate löschen
/*
private func Shoot(object caller) {          // Feuern mit Feuermodus
  //User da? :S
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim)>0 && !(GetUser()->~AimOverride()))
    return(stopauto=true); //abbrechen
  // user hat sich irgendwo reinbewegt 
  if(GetUser()->Contained() && GetUser() == Contained())
    return(stopauto=true);
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto))
    shooting = true;

  // Munition abziehen
  if(ratecount == 1) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { --ratecount; }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = false;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
    	if(!IsReloading())
      	ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}*/


global func CheckEnemy(object pObj,object pObj2) {
	if(!pObj2)
		pObj2 = this();
	
	if(IsInvincible(pObj) || IsInvincible(pObj2))
		return(false);
	
	return(inherited(pObj, pObj2));
}
