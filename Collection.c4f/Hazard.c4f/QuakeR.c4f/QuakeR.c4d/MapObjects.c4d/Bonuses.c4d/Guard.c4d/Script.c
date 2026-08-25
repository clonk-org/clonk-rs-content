/*-- Guard --*/

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

public func Color() { return(RGB(0,255,0)); }

public func Activate(object obj) {
  if(!AddEffect("GuardBonus",obj,190,5,obj,0,time))
    return();
  //Sound("quaddamage", 1, 0,0, GetOwner(obj)+1);

  RemoveObject();
  return(true);
}

global func FxGuardBonusStart(object pTarget, int iEffectNumber, int tmp, int time) {
  if(tmp)
    return();
   
  EffectVar(0,pTarget,iEffectNumber) = time;
}

global func FxGuardBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	var a = iEffectTime;
	CreateParticle("FxShield",0,0,Sin(a,100),Cos(a,100),30*5,RGBa(0,200,0,50),pTarget);
  
  var totaltime = iEffectTime+EffectVar(0,pTarget,iEffectNumber);
  
  if(totaltime > (BONUS_Duration*4)/5)
  	Sound("bonus_wearoff",0,pTarget); 
  
  if(totaltime > BONUS_Duration)
    return(-1);
}

global func FxGuardBonusStop(object pTarget, int iEffectNumber, int reason, int tmp) {
  if(tmp)
    return();
 
 	if(pTarget->GetID() == QBGD)
  	return();
 
  // haben wir gestoppt weil der Clonk gestorben ist?
  if(reason == 3 || reason == 4)
  {
  	// Quaddmg droppen!
  	var bonus = pTarget->CreateObject(QBGD, 0,0,-1);
  	var timeleft = GetEffect(0,pTarget,iEffectNumber,6) + EffectVar(0,pTarget,iEffectNumber);
  	bonus->SetTime(timeleft);
  	AddEffect("GuardBonus",bonus,190,5,bonus,0,timeleft);
  }
}

global func FxGuardBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "GuardBonus")
  {
    return(-1);
  }
}

global func FxGuardBonusDamage(object pTarget, int iEffectNumber, int dmg, int cause) {
  return(dmg/3);
}
