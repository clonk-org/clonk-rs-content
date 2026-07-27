/* Stampfen */

#strict 2

func Activate(pCaster,pRealcaster)
{    
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  if(GetEffect("ControlProjectileShooting", pClonk))
  {
   PlayerMessage(pClonk->GetOwner(), "$AlreadyHas$", pClonk);
   Sound("Error", 0, 0, 0, pClonk->GetOwner());
   return 0;
  }
  
  AddEffect("ControlProjectileShooting",pClonk,250,5*36,0,GetID());
  RemoveObject();
  return 1;
}

func FxControlProjectileShootingStart(pTarget, iEffectNumber, iTemp)
{
	if(iTemp)return;
	EffectVar(0, pTarget, iEffectNumber)=3;
	EffectVar(1, pTarget, iEffectNumber)=CreateObject(HARH, 0, 0, -1);
	ObjectSetAction(EffectVar(1, pTarget, iEffectNumber), "Be", pTarget);
	EffectVar(1, pTarget, iEffectNumber)->Update(5);
}

func FxControlProjectileShootingTimer(pTarget, iEffectNumber, iEffectTime)
{
  EffectVar(0, pTarget, iEffectNumber)--;
  EffectVar(1, pTarget, iEffectNumber)->Update(EffectVar(0, pTarget, iEffectNumber));
  if(EffectVar(0, pTarget, iEffectNumber) > 0) return;
  return -1;
}

func FxControlProjectileShootingStop(pTarget, iEffectNumber, iReason, iTemp)
{
	if(iTemp)return;
	if(!EffectVar(1, pTarget, iEffectNumber))return;
	EffectVar(1, pTarget, iEffectNumber)->Update(0);
	EffectVar(1, pTarget, iEffectNumber)->RemoveObject();
}

func FxControlProjectileShootingShootProjectile(pTarget, iEffectNumber, pWhat)
{
	if(!pWhat)return;
	var dec=1;
	if(pWhat->~IsBullet())return;//dec=2;
	//if(GetID(pWhat) == SBLT)dec=1;
	//else if(GetID(pWhat) == FARW) dec=2;
	
	if(pWhat->~IsArrow()) AddEffect("HomingArrow", pWhat, 1, 1, 0, MHAR, pTarget);
	else if(pWhat->~IsBullet()) AddEffect("HomingBullet", pWhat, 1, 1, 0, MHAR, pTarget);
	
	EffectVar(0, pTarget, iEffectNumber)-=dec;
  EffectVar(1, pTarget, iEffectNumber)->Update(EffectVar(0, pTarget, iEffectNumber));
  if(EffectVar(0, pTarget, iEffectNumber) <= 0)
  {
  	RemoveEffect(0, pTarget, iEffectNumber);
  }
}

func FxHomingArrowStart(pTarget, iEffectNumber, iTemp, par1)
{
	if(iTemp)return;
	EffectVar(0, pTarget, iEffectNumber)=Distance(0,0,GetXDir(pTarget), GetYDir(pTarget));
	EffectVar(1, pTarget, iEffectNumber)=par1;
}

func FxHomingArrowTimer(pTarget, iEffectNumber, iEffectTime)
{
	if(iEffectTime > 36*10)return -1;
	if(GetAction(pTarget) != "Shot")return -1;
	var clonk=EffectVar(1, pTarget ,iEffectNumber);
	var speed=BoundBy(EffectVar(0, pTarget, iEffectNumber)-5,2, 100);
	
	CreateParticle("PSpark", GetX(pTarget), GetY(pTarget), 0, 0, 60, RGBa(255,255,10,100), 0, 0);
	
	var enem;
	for(var obj in FindObjects(Find_Distance(200, pTarget->GetX(), pTarget->GetY()), Find_OCF(OCF_CrewMember), Find_Hostile(GetOwner(pTarget)), Find_NoContainer()))
	{
		if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(obj), GetY(obj)))continue;
		enem=obj;
		break;
	}
	if(!enem)return;
	
	var angle=Angle(GetX(pTarget), GetY(pTarget), GetX(enem), GetY(enem));
	var xDir=Sin(angle, speed);
	var yDir=-Cos(angle, speed);
	var cx=GetXDir(pTarget);
	var cy=GetYDir(pTarget);
	
	//pTarget->SetSpeed((pTarget->GetXDir()*3+xDir)/4, (pTarget->GetYDir()*3+yDir)/6);
	pTarget->SetSpeed(BoundBy(xDir, cx-2,cx+2), BoundBy(yDir, cy-2,cy+2)-2);
}

func FxHomingBulletStart(pTarget, iEffectNumber, iTemp, par1)
{
	if(iTemp)return;
	EffectVar(0, pTarget, iEffectNumber)=Distance(0,0,GetXDir(pTarget), GetYDir(pTarget));
	EffectVar(1, pTarget, iEffectNumber)=par1;
}

func FxHomingBulletTimer(pTarget, iEffectNumber, iEffectTime)
{
	if(iEffectTime > 36*3)return -1;
	if(GetAction(pTarget) != "Shot")return -1;
	var clonk=EffectVar(1, pTarget ,iEffectNumber);
	var speed=BoundBy(EffectVar(0, pTarget, iEffectNumber)-20,2, 100);
	CreateParticle("PSpark", GetX(pTarget), GetY(pTarget), 0, 0, 60, RGBa(255,255,10,150), pTarget);
	
	var enem;
	var ang=Angle(0,0,GetXDir(pTarget), GetYDir(pTarget));
	for(var obj in FindObjects(Find_Distance(250, pTarget->GetX(), pTarget->GetY()), Find_OCF(OCF_CrewMember), Find_Hostile(GetOwner(pTarget))))
	{
		if(!PathFree(GetX(pTarget), GetY(pTarget), GetX(obj), GetY(obj)))continue;
		if(!Inside(Angle(GetX(pTarget), GetY(pTarget), GetX(obj), GetY(obj)), ang-50, ang+50))continue;
		enem=obj;
		break;
	}
	if(!enem)return;

	var angle=Angle(GetX(pTarget), GetY(pTarget), GetX(enem), GetY(enem));
	var xDir=Sin(angle, speed);
	var yDir=-Cos(angle, speed);
	
	pTarget->SetSpeed((pTarget->GetXDir()*2+xDir)/3, (pTarget->GetYDir()*2+yDir)/3);
}

// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Nur für die Lichtkirche
//public func NeedChurch() { return scLightchurch; }

func HasItemRequirements(){return BOW1;}


// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
