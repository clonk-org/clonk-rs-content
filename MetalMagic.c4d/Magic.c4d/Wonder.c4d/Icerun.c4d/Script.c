/* Stampfen */

#strict 2

func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  AddEffect("IceRunRejectingFights",pClonk,250,5,0,GetID());
  if(pClonk->~IsFighting())
  {
  	pClonk->~StopFighting();
  }
  RemoveObject();
  return 1;
}

func FxIceRunRejectingFightsStart(pTarget, iEffectNumber, iTemp)
{
	if(iTemp)return;
  
  // anti freezing the guy
  SetPhysical("Walk",(GetPhysical("Walk",0,pTarget)*3)/2, PHYS_StackTemporary, pTarget);
}

func FxIceRunRejectingFightsTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(iEffectTime > 35*3) return -1;
  
  CreateParticle("Freeze",GetX(pTarget),GetY(pTarget),0,0,20*5+20,RGB(255,40,80),pTarget);
  
  if(GetContact(pTarget,-1,CNAT_Bottom))
  {
  	CreateParticle("PxSpark",GetX(pTarget)+RandomX(-5,1),GetY(pTarget)+9,RandomX(-2,0),-20,60,RGB(50,50,255),pTarget,Random(2));
  	CreateParticle("PxSpark",GetX(pTarget)+RandomX(-1,5),GetY(pTarget)+9,RandomX(0,2),-20,60,RGB(50,50,255),pTarget,Random(2));
  }

	// Freezing near enemies?
	for(var obj in FindObjects(Find_AtPoint(GetX(pTarget),GetY(pTarget)), Find_OCF(OCF_Alive), Find_Hostile(GetOwner(pTarget))))
	{
		// already cold?
		if(GetEffect("*Freeze", obj))continue;
		// you're so terribly cool now
		Freeze(obj,  pTarget, 35*2);
	}

  return 1;
}

func FxIceRunRejectingFightsStop(pTarget,iEffectNumber, iTemp)
{
	if(iTemp)return;
	ResetPhysical(pTarget, "Walk");
}


// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Nur für die Lichtkirche
//public func NeedChurch() { return scLightchurch; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("222"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
