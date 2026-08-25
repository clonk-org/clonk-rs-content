/*-- Respawnen & Co --*/

#strict 2

#appendto ASAS

protected func Initialize()
{
  AddEffect("DamageCounter",this,20,5,0,GetID());
  return _inherited(...);
}

protected func MenuQueryCancel()
{
  // Waffenauswahl nicht abbrechen
  if (GetMenu() == WIPF) return 1;
}

protected func Death(int iCausePlr)
{
  Sound("Die"); 
  if (Not(GetCrew(GetOwner())))
    GameCall("RelaunchPlayer",GetOwner(), iCausePlr);
  DeathAnnounce();

  if(iCausePlr != GetOwner() && GetPlayerName(iCausePlr))
    AddEffect("DisplayMoney",CreateObject(DSHP,0,-5,-1),20,3,0,GetID(),15,iCausePlr);

  ++g_No_Relaunch_Removal;
  _inherited(iCausePlr, ...);
  --g_No_Relaunch_Removal;
}

protected func FxDamageCounterStart()
{
  return 1;
}

protected func FxDamageCounterDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy > 0) return iDmgEngy;
 
  if(GetEnergy(pTarget)<=Abs(iDmgEngy)/1000)
  {  
    for(var cnt=0;cnt<ContentsCount(0,pTarget);cnt++)
    {
      if(GetID(Contents(0,pTarget)) == CRBW || !Random(3))RemoveObject(Contents(cnt,pTarget));
    }
 
    var rand = Random(1)+1;
    //if(!rand) CreateObject(PBLP,GetX(pTarget),GetY(pTarget),-1);
    //if(!--rand) CreateObject(SBLP,GetX(pTarget),GetY(pTarget),-1);
    if(!--rand)CreateObject(BRED,GetX(pTarget),GetY(pTarget),-1);
    //if(!--rand)CreateObject(TSWD,GetX(pTarget),GetY(pTarget),-1);
  }
 
  var iKiller = GetKiller(pTarget);
  if(iKiller !=-1 && iKiller != GetOwner(pTarget))
    Local(0, pTarget) = iKiller;
  EffectVar(0, pTarget, iEffectNumber) = 7;

  return iDmgEngy;
}

protected func FxDamageCounterTimer(object pTarget, int iEffectNumber)
{
  if(EffectVar(0, pTarget, iEffectNumber) && GetKiller()==-1)
  {
    if(!--EffectVar(0, pTarget, iEffectNumber))
      Local(0, pTarget) = GetOwner(pTarget);
  }
  return 1;
}

protected func FxFadeOutTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(iEffectTime > 250){ RemoveObject(pTarget); return -1; }
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


