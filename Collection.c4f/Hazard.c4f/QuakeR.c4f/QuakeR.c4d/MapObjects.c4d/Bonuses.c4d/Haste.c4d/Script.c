/*-- Haste --*/

#strict
#include HSTB

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

public func Color() { return(RGB(255,255,0)); }

public func Activate(object obj) {
  if(!AddEffect("HasteBonus",obj,190,5,obj,0,time))
    return();
  Sound("haste", 1, 0,0, GetOwner(obj)+1);
  RemoveObject();
  return(true);
}

global func FxHasteBonusStart(object pTarget, int iEffectNumber, int tmp, int time) {
  if(tmp)
    return();
  pTarget->SetPhysical("Walk",pTarget->GetPhysical("Walk") + 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Jump",pTarget->GetPhysical("Jump") + 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Scale",pTarget->GetPhysical("Scale") + 10000,PHYS_StackTemporary);
  pTarget->SetPhysical("Hangle",pTarget->GetPhysical("Hangle") + 10000,PHYS_StackTemporary);
  
  EffectVar(0,pTarget,iEffectNumber) = time;
}

global func FxHasteBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("NoGravSpark",RandomX(-5,5),RandomX(-6,6),0,0,35,RGB(250,250,Random(20)),0,Random(2));
  
  var totaltime = iEffectTime+EffectVar(0,pTarget,iEffectNumber);
  
  if(totaltime > (BONUS_Duration*4)/5)
  	Sound("bonus_wearoff",0,pTarget); 
  
  if(totaltime > BONUS_Duration)
    return(-1);
}

global func FxHasteBonusStop(object pTarget, int iEffectNumber, int reason, int tmp) {
  if(tmp)
    return();
  pTarget->SetPhysical("Walk",pTarget->GetPhysical("Walk") - 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Jump",pTarget->GetPhysical("Jump") - 15000,PHYS_StackTemporary);
  pTarget->SetPhysical("Scale",pTarget->GetPhysical("Scale") - 10000,PHYS_StackTemporary);
  pTarget->SetPhysical("Hangle",pTarget->GetPhysical("Hangle") - 10000,PHYS_StackTemporary);
  
  if(pTarget->GetID() == QBHS)
  	return();
  
  // haben wir gestoppt weil der Clonk gestorben ist?
  if(reason == 3 || reason == 4)
  {
  	// Quaddmg droppen!
  	var haste = pTarget->CreateObject(QBHS, 0,0,-1);
  	var timeleft = GetEffect(0,pTarget,iEffectNumber,6) + EffectVar(0,pTarget,iEffectNumber);
  	haste->SetTime(timeleft);
  	AddEffect("HasteBonus",haste,190,5,haste,0,timeleft);
  }
}

global func FxHasteBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "HasteBonus")
  {
    return(-1);
  }
}

