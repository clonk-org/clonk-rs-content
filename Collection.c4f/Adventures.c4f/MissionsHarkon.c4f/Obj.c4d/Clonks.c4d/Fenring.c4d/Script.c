/*-- Fenring --*/

#strict 2

#include CLNK

local pBossMeter;

local pEnemy;

local Magic;

func StartFight()
{
	AddEffect("BossFight", this, 1, 1, this);
}

func FxExplosionProtectionDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
	if(iCause == FX_Call_EngBlast)
	{//Log("!%d",iDmgEngy);
	  return iDmgEngy/5;
	}
	return iDmgEngy;
}

func FxBossFightStart()
{
	SetOwner(10);
	pBossMeter = CreateObject(BOSS, 290-GetX(), 1040-GetY(), -1);
	SetPhysical("Energy", 100*1000, 2);
	DoEnergy(100);
	CreateObject(_SUL, 389-GetX(), 921-GetY(), -1)->Activate();
	CreateObject(_SUL, 558-GetX(), 907-GetY(), -1)->Activate();
	CreateObject(_SUL, 686-GetX(), 907-GetY(), -1)->Activate();
/*	
	CreateObject(_FBB, 593-GetX(), 868-GetY());
	CreateObject(_FBB, 439-GetX(), 907-GetY());
	CreateObject(_FBB, 671-GetX(), 899-GetY());*/
	SetPhysical("Walk", 40000, 2);
	for(obj in FindObjects(Find_ID(ASAS)))
	{
	  AddEffect("ExplosionProtection", obj, 20, 0, this);
	  obj->SetPhysical("Walk", 90000, PHYS_Temporary);
	}
}

local pSpecialObject;
local iItemCounter;

func FxBossFightStop(pTarget, iNumber, iReason)
{
	if(iReason == 4)
	{
		// Feuerbälle
		RemoveAll(FRBL);
		RemoveAll(_FBB);
		RemoveAll(_MFB);
		// Flints
		RemoveAll(SFLN);
		RemoveAll(_TFN);
	}
}

func FxBossFightTimer()
{
	if(!pEnemy || Random(10))
		pEnemy = FindObject2(Find_Distance(500), Find_OCF(OCF_Alive), Sort_Distance(), Find_Hostile(GetOwner()), Find_Layer(GetObjectLayer()));

	if(!pEnemy) return;
	
	if(!pSpecialObject)
	{
	  var rand = Random(3);
	  var x = [506, 605, 697][rand];
	  var y = [917, 878, 909][rand];
	  if(iItemCounter>10)
		{
	    pSpecialObject = CreateObject(PHEA, x-GetX(), y-GetY());
			iItemCounter = 0;
		}
//	  else if(!Random(8))
//	    pSpecialObject = CreateObject(PWIP, x-GetX(), y-GetY());
	  else if(!Random(3))
	    pSpecialObject = CreateObject(SFLN, x-GetX(), y-GetY());
	  else
	    pSpecialObject = CreateObject(_TFN, x-GetX(), y-GetY());
		iItemCounter++;
	}
	if( (GetID(pSpecialObject) == PHEA || GetID(pSpecialObject) == PWIP) && !Random(200))
	{
	  pSpecialObject->PoffClonk();
	  pSpecialObject->RemoveObject();
	}
	if(!ObjectCount(_FBB) && !Random(100))
	{
	  var rand = Random(3);
	  var x = [593, 439, 671][rand];
	  var y = [917, 878, 909][rand];
	  CreateObject(_FBB, x-GetX(), y-GetY(), GetOwner());
	}
	
/*	if(ObjectDistance(pEnemy)<5 && pEnemy != this && 0)
	{
		//Fling(pEnemy, -2+5*GetDir(), -3);
		SetAction("Magic");
		Sound("Blast2");
		DoEnergy(-4,pEnemy);
		//Fling(this, -2-5*GetDir(), -3);
		for(var i=0;i<5;++i)
		  CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,5),0,0,RandomX(80,200),RGB(255,40,0));
		return;
	}*/
	if(!Random(80))
		Jump();
	
	if(!Random(10) && ObjectDistance(pEnemy) > 20)
	{
	  var iDir = 0;
	  if(GetX() < pEnemy->GetX()) iDir = 1;
	    SetDir(iDir);
	  var dir = COMD_Left;
	  if(iDir) dir = COMD_Right;
	  SetComDir(dir);
	}
	if(ObjectDistance(pEnemy) < 15) Jump();
	
	var iSpellCost = 80;
	if(Magic < iSpellCost*2) Magic++;
	
	if(!Random(60) && GetAction() == "Walk" && Magic >= iSpellCost && !ObjectCount(FRBL) && GetEnergy()<60)
	{
		Magic -= iSpellCost;
		SetAction("Magic");
		for(var i = 0; i < 3; i++)
			CreateObject(FRBL, 0, 0, GetOwner());
	}
	
	if(!Random(60) && GetAction() == "Walk" && Magic >= iSpellCost)
	{
		Magic -= iSpellCost;
		SetAction("Magic");
		CreateObject(_MFB)->Activate(this);
	}
	if(!Random(60) && GetAction() == "Jump" && Magic >= iSpellCost)
	{
		Magic -= iSpellCost;
		SetAction("JumpMagic");
		CreateObject(_MFB)->Activate(this);
	}
}

func FxBossFightDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
	pBossMeter->SetPercent(GetEnergy()+iDmgEngy/1000);
	return iDmgEngy;
}

func DoSpellAim(pSpell)
{
	var iAngle = Angle(GetX(), GetY(), pEnemy->GetX(), pEnemy->GetY());
	pSpell->ActivateAngle(this, iAngle);
	return 1;
}