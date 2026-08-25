/*-- Respawnen & Co --*/

#strict 2

#appendto MAGE

protected func Initialize()
{
  AddEffect("DamageCounter",this,20,5,0,GetID());
  return _inherited();
}

protected func MenuQueryCancel()
{
  // Waffenauswahl nicht abbrechen
  if (GetMenu() == WIPF) return 1;
}

protected func Death(iBy)
{
  iBy=Local();
  if(iBy != GetOwner() && GetPlayerName(iBy))
  {
    DoMagicEnergy(100,GetCursor(iBy), 1);
    AddEffect("DisplayMoney",CreateObject(DSHP,0,-5,-1),20,3,0,GetID(),15,iBy);
 
    EffectCall(0,GetEffect("FragsSaver",0),"AddFrag",iBy);
  }
 
  if(GetOwner() == iBy)
    EffectCall(0,GetEffect("FragsSaver",0),"RemFrag",iBy);
 
  AddEffect("FadeOut",this,20,5,0,GetID());
  
  return _inherited();
}

protected func FxDamageCounterStart()
{
  return 1;
}

protected func FxDamageCounterDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy > 0) return iDmgEngy;

  var iKiller = GetKiller(pTarget);
  if(iKiller !=-1 && iKiller != GetOwner(pTarget))
    Local(0, pTarget) = iKiller;
  EffectVar(0, pTarget, iEffectNumber) = 7;

  return iDmgEngy;
}

protected func FxDamageCounterTimer(object pTarget, int iEffectNumber)
{
  if(EffectVar(0, pTarget, iEffectNumber) && GetKiller(pTarget)==-1)
  {
    if(!--EffectVar(0, pTarget, iEffectNumber))
      Local(0, pTarget) = GetOwner(pTarget);
  }
  return 1;
}

protected func FxDamageCounterStop(pTarget, iEffectNumber, iReason, fTmp)
{
  if(fTmp) return;
  // Beim Tod den Stab vernichten
/*  if(LocalN("idStaff", pTarget))
  {
    LocalN("idStaff", pTarget) = 0;
    SetGraphics(0, this, 0, 0);
    SetGraphics(0, this, 0, 1);
    if(LocalN("pStaffPict", pTarget)) RemoveObject(LocalN("pStaffPict", pTarget));
  }*/
}

protected func FxFadeOutTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(iEffectTime > 250){ RemoveObject(pTarget); return(-1); }
  SetClrModulation(RGBa(255,255,255,iEffectTime),pTarget);
  return 1;
}

protected func FxDisplayMoneyStart(pTarget,iEffectNumber,iTemp,val1,val2)
{
  EffectVar(0,pTarget,iEffectNumber)=val1;
  EffectVar(1,pTarget,iEffectNumber)=val2;
}

protected func FxDisplayMoneyTimer(pTarget,iEffectNumber,iEffectTime)
{
  Message("<c eeee00>%d",pTarget,EffectVar(0,pTarget,iEffectNumber));
  if(--EffectVar(0,pTarget,iEffectNumber) < 0) return -1;
 
  DoWealth(EffectVar(1,pTarget,iEffectNumber),1);
 
  return 1;
}

protected func FxDisplayTimerStop(pTarget,iEffectNumber,iEffectTim)
{
  if(pTarget) RemoveObject(pTarget);
  CastParticles("PSpark",4,5,GetX(pTarget),GetY(pTarget),40,100,RGB(200,200,50),RGB(255,255,70));
  CastParticles("PxSpark",4,5,GetX(pTarget),GetY(pTarget),40,70,RGB(200,200,50),RGB(255,255,70));
}


