/* Kampfschrei */

#strict 2

static const MBCY_Range=100;

func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  AddEffect("BattleCryEffect",pClonk,250,1,0,GetID());
	
	Sound("Scream", 0, pClonk);
	for(var obj in FindObjects(Find_Distance(MBCY_Range, AbsX(GetX(pClonk)),AbsY(GetY(pClonk))), Find_OCF(OCF_Alive), Find_Hostile(GetOwner(pClonk))))
	{
		AddEffect("BattleCryAmplifyDamage", obj, 1, 5, 0, GetID());
	}
	
  RemoveObject();
  return 1;
}


func FxBattleCryAmplifyDamageTimer(pTarget, iEffectNumber, iEffectTime)
{
	if(iEffectTime > 35*10)return -1;
	CreateParticle("BattleCry",GetX(pTarget),GetY(pTarget)-15,0,0,70+RandomX(-10,20),RGB(200,150,30),pTarget);
}

func FxBattleCryAmplifyDamageDamage(pTarget, iEffectNumber, iDmgEngy, iCause)
{
	if(iDmgEngy > 0)return iDmgEngy;
	return (iDmgEngy*3)/2;
}

func FxBattleCryEffectTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(iEffectTime > 18*1) return -1;
  
  CreateParticle("BattleCry",GetX(pTarget),GetY(pTarget),0,0,(iEffectTime*900)/18,RGB(90+iEffectTime*9,150-iEffectTime*8,36-iEffectTime*2),pTarget);
  
  

  return 1;
}

// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Nur für die Blutkirche
//public func NeedChurch() { return scBloodchurch; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
