/*-- RapidFire --*/

#strict

local time;

func SetTime(int newtime) { time = newtime; }

func Initialize()
{
	SetAction("Be");
}

func Check()
{
	if(Contained())
		return();
	
	if(time + GetActTime() > BONUS_Duration)
		RemoveObject();
}

func RejectEntrance(object byObj)
{
	if(GetOCF(byObj) & OCF_CrewMember)
	{
		time += GetActTime();
		Activate(byObj);
	}
	if(GetID(byObj) == BSPN)
		return();
	return(1);
}

public func IsBonus() { return(true); }

public func Color() { return(RGB(255,0,0)); }

public func Activate(object obj) {
  if(!AddEffect("RapidFireBonus",obj,190,3,obj,0,time))
    return();
  //Sound("quaddamage", 1, 0,0, GetOwner(obj)+1);
  
  var def;
  for(var i = 0; def = GetDefinition(i); i++)
  	if(def->~IsAmmo() && def != QRDS && def != QBFS)
  	{
  		if(WildcardMatch(Format("%i",def),"Q??S"))
  		{
  			DoAmmo(def, -GetAmmo(def,obj), obj);
  			DoAmmo(def, def->MaxAmmo(), obj);
  		}
  	}
  
  obj->~OnAmmoChange();
  RemoveObject();
  return(true);
}

global func FxRapidFireBonusStart(object pTarget, int iEffectNumber, int tmp, int time) {
  if(tmp)
    return();
   
  EffectVar(0,pTarget,iEffectNumber) = time;
}

global func FxRapidFireBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	var a = iEffectTime*6;
  CreateParticle("NoGravSpark",Sin(a,10),9,0,-4,35,RGB(200,20,20),pTarget, a > 90 && a <= 270);
  
  var totaltime = iEffectTime+EffectVar(0,pTarget,iEffectNumber);
  
  if(totaltime > (BONUS_Duration*4)/5)
  	Sound("bonus_wearoff",0,pTarget); 
  
  if(totaltime > BONUS_Duration)
    return(-1);
}

global func FxRapidFireBonusStop(object pTarget, int iEffectNumber, int reason, int tmp) {
  if(tmp)
    return();
 
 	pTarget->~OnAmmoChange();
 
 	if(pTarget->GetID() == QBRF)
  	return();
 
  // haben wir gestoppt weil der Clonk gestorben ist?
  if(reason == 3 || reason == 4)
  {
  	// Quaddmg droppen!
  	var bonus = pTarget->CreateObject(QBRF, 0,0,-1);
  	var timeleft = GetEffect(0,pTarget,iEffectNumber,6) + EffectVar(0,pTarget,iEffectNumber);
  	bonus->SetTime(timeleft);
  	AddEffect("RapidFireBonus",bonus,190,5,bonus,0,timeleft);
  }
}

global func FxRapidFireBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "RapidFireBonus")
  {
    return(-1);
  }
}

global func FxRapidFireBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Speeeeed!
  if(iData == FM_Reload) return(value/2);
  if(iData == FM_Recharge) return(value/2);
  return(value);
}
