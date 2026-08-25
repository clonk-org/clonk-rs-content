/*-- QuadDamage --*/

#strict

//Dauer: die halbe * die ganze Antwort!
static const BONUS_Duration = 882;

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

public func Color() { return(RGB(0,100,255)); }

public func Activate(object obj) {
  if(!AddEffect("QuadDamageBonus",obj,190,5,obj,0,time))
    return();
  Sound("quaddamage", 1, 0,0, GetOwner(obj)+1);
  RemoveObject();
  return(true);
}

global func FxQuadDamageBonusStart(object pTarget, int iEffectNumber, int tmp, int time) {
  if(tmp)
    return();
  SetObjectBlitMode(GFX_BLIT_ClrSfc_Mod2);
  SetClrModulation(RGB(100,200,255),pTarget);
  EffectVar(0,pTarget,iEffectNumber) = time;
}

global func FxQuadDamageBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("NoGravSpark",0,0,RandomX(-4,4),RandomX(-4,4),35,RGB(0,100,255),pTarget,1);
  
  var totaltime = iEffectTime+EffectVar(0,pTarget,iEffectNumber);
  
  if(totaltime > (BONUS_Duration*4)/5)
  	Sound("bonus_wearoff",0,pTarget); 
  
  if(totaltime > BONUS_Duration)
    return(-1);
}

global func FxQuadDamageBonusStop(object pTarget, int iEffectNumber, int reason, int tmp) {
  if(tmp)
    return();
  SetClrModulation(RGB(255,255,255),pTarget);
  SetObjectBlitMode(0);
 
 	if(pTarget->GetID() == QBQD)
  	return();
 
  // haben wir gestoppt weil der Clonk gestorben ist?
  if(reason == 3 || reason == 4)
  {
  	// Quaddmg droppen!
  	var qdmg = pTarget->CreateObject(QBQD, 0,0,-1);
  	var timeleft = GetEffect(0,pTarget,iEffectNumber,6) + EffectVar(0,pTarget,iEffectNumber);
  	qdmg->SetTime(timeleft);
  	AddEffect("QuadDamageBonus",qdmg,190,5,qdmg,0,timeleft);
  }
}

global func FxQuadDamageBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "QuadDamageBonus")
  {
    return(-1);
  }
}

global func FxQuadDamageBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Damage*3!
  if(iData == FM_Damage) return(value*3);
  return(value);
}
