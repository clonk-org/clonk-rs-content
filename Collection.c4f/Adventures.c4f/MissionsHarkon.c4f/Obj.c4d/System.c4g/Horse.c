/*-- Pferd --*/

#appendto HORS
#strict 2

public func ControlAllowed(pObj)
{
	if(GetRider() && pObj->GetAction() == "Push")
		return 0;
	return 1;
}

public func ControlLeft(pByObject)
{ 
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}

public func ControlRight(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}

public func ControlUpdate(object pByObject, int comdir)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, comdir, ...);
}

public func ControlLeftReleased(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}

public func ControlRightReleased(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}

public func ControlUp(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);         
}

public func ControlDownSingle(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}

public func ControlDownDouble(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}

public func ControlDigDouble(pByObject)
{
	if(!ControlAllowed(pByObject)) return 0;
	return _inherited(pByObject, ...);
}