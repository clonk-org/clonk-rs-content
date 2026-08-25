#strict
#include STAP

public func Initialize()
{
	_inherited();
	SetAction("Turn");
}

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(QMGS); }
public func AmmoCount()    { return(100); }

func OnTransfer()
{
	Sound("CollectAmmo");
}

public func MayTransfer(object pObj)
{
  if(!pObj) return(false);
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) >= MaxAmmo)
      return(false);
  return(true);
}

public func TransferAmmo(object pObj)		// Ammo dem Clonk geben
{
  if(!pObj) return(false);
  if(NoAmmo()) return(false);
  
  var ammount;
  var maxammo = AmmoID()->~MaxAmmo();
  ammount = GetAmmo(AmmoID(),pObj);
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj)) {
    //PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return();
  }

	if(ammount + AmmoCount() > maxammo)
	{
		ammount = AmmoCount() - ((ammount + AmmoCount()) - maxammo);
	}
	else
		ammount = AmmoCount();

  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());
  DoAmmo(AmmoID(),ammount,pObj);
  pObj->~AmmoTransferred();
  if(!OnTransfer())
  	RemoveObject();

  return(true);
}
