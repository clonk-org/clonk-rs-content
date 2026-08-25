/*-- Teleport --*/

#strict

func Initialize()
{
	SetAction("Be");
}

func RejectEntrance(object byObj)
{
	if(GetOCF(byObj) & OCF_CrewMember)
	{
		Activate(byObj);
	}
	if(GetID(byObj) == BSPN)
		return();
	return(1);
}

public func IsBonus() { return(true); }

public func Color() { return(RGB(0,0,128)); }

public func Activate(object obj) {
  if(!AddEffect("TeleportBonus",obj,190,0,obj))
    return();
  //Sound("quaddamage", 1, 0,0, GetOwner(obj)+1);

  RemoveObject();
  return(true);
}

global func FxTeleportBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "TeleportBonus")
  {
    return(-1);
  }
}
