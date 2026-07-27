/* Kampfeshunger */

#strict 2



func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  AddEffect("BattleHungerForcesToFightWith",pClonk,250,2,0,GetID());
		
  RemoveObject();
  return 1;
}


func FxBattleHungerForcesToFightWithTimer(pTarget, iEffectNumber, iEffectTime)
{  
  if(iEffectTime > 5*35) return -1;
  CreateParticle("BattleHunger",GetX(pTarget),GetY(pTarget),RandomX(-2,2),RandomX(-2,2),Cos(iEffectTime*10, 60)+100,RGB(Sin(iEffectTime*10, 128)+128,100-Cos(iEffectTime*9, 100),1),pTarget, 1);
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
