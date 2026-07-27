/* Silberner Schlüssel */

#strict 2

local iRefID;

/* Alleinstehender Schlüssel */

protected func Initialize()
	{
	SetAction("Key1");
	SetRefID(Random(15));
  }

/* Statussetzung */

public func SetRefID(int iID)
	{
	iRefID = iID;
	return 1;
	}

public func GetRefID() { return iRefID; }

public func RefIDMatch(int iID) { return iID==iRefID; }

/* Identifizierung */

public func Activate()
	{
  [$TxtInfo$|Image=_SKE]
  Message("Ref ID #%d|made in Claiwan", this, GetRefID());
  return 1;
  }

/* Aufschlag */

protected func Hit()
  {
  Sound("Crystal*");
  }

/* Status */

public func IsKey() { return 1; }

/* Produkteigenschaften */

public func IsAnvilProduct() { return 1; }
