/*-- Redeemer --*/

#strict

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

public func Color() { return(RGB(0,0,255)); }

public func Activate(object obj) {
	var eff;
  if(!(eff = AddEffect("RedeemerBonus",obj,190,5,obj)))
    return();
  
  var bfg = CreateContents(Q_RD,obj);
  
  RemoveObject();
  return(true);
}

global func FxRedeemerBonusStart(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
}

global func FxRedeemerBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  if(!FindContents(Q_RD,pTarget))
    return(-1);
}

global func FxRedeemerBonusStop(object pTarget, int iEffectNumber, int reason, int tmp) {
  if(tmp)
    return();
  
  // haben wir gestoppt weil der Clonk gestorben ist?
  if(reason == 3 || reason == 4)
  {
  	// Redeemer droppen!
  	pTarget->CreateObject(QBRD, 0,0,-1);
  }
}

global func FxRedeemerBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "RedeemerBonus")
  {
    return(-1);
  }
}
