/* Double Arrows */

#strict 2


func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  if(GetEffect("ControlDoubleArrows", pClonk))
  {
  	Sound("Error", 0, 0, 0, GetOwner(pClonk)+1);
  	PlayerMessage(GetOwner(pClonk), "$AlreadyHas$", pClonk);
  	return 0;
  }
  
	AddEffect("ControlDoubleArrows", pClonk, 1, 100, 0, MDAR);
	RemoveObject();
	return 1;
}

func FxControlDoubleArrowsStart(pTarget, iEffectNumber, iTemp)
{
	if(iTemp)return;
}

func FxControlDoubleArrowsTimer(pTarget, iEffectNumber, iEffectTime)
{
  return -1;
}

func FxControlDoubleArrowsStop(pTarget, iEffectNumber, iReason, iTemp)
{
	if(iTemp)return;
}

func FxControlDoubleArrowsShootProjectile(pTarget, iEffectNumber, pWhat)
{
	if(!pWhat)return;
	if(pWhat->~IsBullet())return;
	//if(GetEffect("*Silenced*", pTarget))return;
	if(GetEffect("CooldownDoubleArrow", pTarget))return;
	//var chance=5+GetStat(GetOwner(pTarget), AGI)+GetSkillStrength(GetOwner(pTarget), MDAR)*4;
	
	//if(Random(100) <= chance)return;
	
	// zweifachschuss!
	AddEffect("CooldownDoubleArrow", pTarget, 1, 5);
	
	var bow=Contents(0, pTarget);
	if(GetID(bow) != BOW1)
		bow=FindObject2(Find_Container(pTarget), Find_ID(BOW1));
	
	if(bow)
	{
		pTarget->SetAction("AimBow");
		bow->SetAction("Idle");
		bow->ControlThrow(pTarget);
		EffectVar(1, pTarget, iEffectNumber)++;
		if(EffectVar(1, pTarget, iEffectNumber) > 5)
		{
			RemoveEffect(0, pTarget, iEffectNumber);
			return;
		}
	}
	
}

func IsPassive(){return true;}

// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Nur für die Lichtkirche
//public func NeedChurch() { return scBloodchurch; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

func HasItemRequirements(){return BOW1;}

public func IsWonder(){return true;}
