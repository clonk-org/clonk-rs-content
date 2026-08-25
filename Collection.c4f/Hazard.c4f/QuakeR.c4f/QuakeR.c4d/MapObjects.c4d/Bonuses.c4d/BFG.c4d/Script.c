/*-- BFG --*/

#strict

local shots;

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

func Initialize()
{
	shots = 5;
}

public func IsBonus() { return(true); }

public func Color() { return(RGB(100,150,255)); }

public func Activate(object obj) {
	var eff;
  if(!(eff = AddEffect("BFGBonus",obj,190,5,obj)))
    return();
  
  var bfg = CreateContents(Q_BF,obj);
  if(shots != 5)
  	bfg->DoAmmo(QBFS,shots-5);
  
  RemoveObject();
  return(true);
}

global func FxBFGBonusStart(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return();
}

global func FxBFGBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  if(!FindContents(Q_BF,pTarget))
    return(-1);
}

global func FxBFGBonusStop(object pTarget, int iEffectNumber, int reason, int tmp) {
  if(tmp)
    return();
  
  // haben wir gestoppt weil der Clonk gestorben ist?
  if(reason == 3 || reason == 4)
  {
  	// Quaddmg droppen!
  	var bfg = pTarget->CreateObject(QBBF, 0,0,-1);
  	bfg->LocalN("shots") = GetAmmo(QBFS,FindContents(Q_BF,pTarget));
  }
}

global func FxBFGBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "BFGBonus")
  {
    return(-1);
  }
}
