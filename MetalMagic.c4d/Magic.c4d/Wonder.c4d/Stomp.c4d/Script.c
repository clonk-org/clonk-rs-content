/* Stampfen */

#strict 2

func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  // has to stand on the ground
  if(!GetContact(pClonk,-1,CNAT_Bottom) && GetYDir(pClonk) > 0/* !pClonk->~IsWalking()*/)
  if(GetAction(pClonk) != "Jump")
  {
   PlayerMessage(pClonk->GetOwner(), "$OnTheGround$", pClonk);
   Sound("Error", 0, 0, 0, pClonk->GetOwner());
   return 0;
  }
  
  AddEffect("Stomping",pClonk,250,2,0,GetID());
  RemoveObject();
  return 1;
}

func FxStompingStart(pTarget, iEffectNumber, iTemp)
{
	if(iTemp)return;
  // target can do some special action?
  if(!GBackSolid(GetX(pTarget)-GetX(), GetY(pTarget)-GetY()-20))
  	{
  		pTarget->SetAction("Jump");
  		if(!Stuck(pTarget))
  		{
  			pTarget->SetPosition(pTarget->GetX(), pTarget->GetY()-1);
  			if(Stuck(pTarget)) pTarget->SetPosition(pTarget->GetX(), pTarget->GetY()+1);
  		}
  		pTarget->SetSpeed(pTarget->GetXDir()/3, pTarget->GetYDir()/2-40);
  	}
}

func FxStompingTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(iEffectTime > 35*5) return -1;
  
  if(GetContact(pTarget,-1,CNAT_Bottom))
  {
  	ShakeViewPort(40, pTarget);
  	Schedule(Format("Sound(\"RockHit*\", 0, Object(%d))",ObjectNumber(pTarget)),1,3);
  	
  	// stunning clonks
  	for(var obj in FindObjects(Find_Distance(60,GetX(pTarget)-GetX(), GetY(pTarget)-GetY()), Find_Exclude(pTarget), Find_OCF(OCF_Alive)))
  	{
  		if((GetContact(obj, -1) & CNAT_Bottom))
  		{
  			 AddEffect("Stun", obj, 1, 1, 0, MSMT, 36*3);
  			continue;
  		}
  		else if(GetContact(obj, -1))
  		{
  			obj->SetAction("Tumble");
  		}
  		
  	}
  	
  	return -1;
  }
  else
  if(iEffectTime > 10)
  	pTarget->SetYDir(pTarget->GetYDir()+10);

  return 1;
}

func FxStunStart(pTarget,iEffectNumber,iTemp, v1)
{
 if(iTemp)return;
 pTarget->SetAction("Tumble");
 pTarget->SetYDir(-20);
 EffectVar(0, pTarget, iEffectNumber)=v1;
}

func FxStunTimer(pTarget,iEffectNumber,iEffectTime)
{
	if(iEffectTime > EffectVar(0, pTarget, iEffectNumber))return -1;
	if(GetAction(pTarget) == "KneelUp")
	{
		pTarget->SetPhase(0);
		if(!Random(10))
		CreateParticle("PSpark",GetX(pTarget)+RandomX(-5,5), GetY(pTarget)-5-Random(4), 0, -2, 80, RGB(50,50,50),pTarget);
	}
	
}

func FxStunDamage(pTarget, iEffectNumber, dmg)
{
	if(dmg < -10*1000)
	{
		// max time = 0
		EffectVar(0, pTarget, iEffectNumber)=0;
	}
	
	return dmg;
} 

// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Nur für die Lichtkirche
//public func NeedChurch() { return scLightchurch; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("522"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
