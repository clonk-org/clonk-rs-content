#strict
#include QMGA

public func Initialize()
{
	_inherited();
	SetAction("Turn");
}

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(QPGS); }
public func AmmoCount()    { return(60); }

func OnTransfer()
{
	Sound("CollectAmmo");
}
