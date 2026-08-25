/*-- Grünzeug --*/


#strict

protected func Initialize()
  {
  SetAction("Be");
  return(1);
  }

protected func AddThing(id)
{
 var obj=CreateObject(id,0,0,-1);
 AddEffect("HoldObject",obj,20,1,this(),0,this());
}

protected func FxHoldObjectStart(pTarget,iNum,iTemp,ithis)
{
 EffectVar(0,pTarget,iNum)=ithis;
 SetPosition(GetX(ithis)+5,GetY(ithis)-3,pTarget);
 EffectVar(1,pTarget,iNum)=GetCategory(pTarget);
 //pTarget->SetCategory(1);
 //SetObjectOrder(pTarget,ithis,0);
 //SetObjectOrder(ithis,pTarget,1);
}

protected func FxHoldObjectTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(!EffectVar(0,pTarget,iEffectNumber) || !pTarget)return(-1);
 if(Contained(pTarget))return(-1);
 SetPosition(GetX(EffectVar(0,pTarget,iEffectNumber))+5,GetY(EffectVar(0,pTarget,iEffectNumber))-3,pTarget);
 SetYDir(-1,pTarget);
 SetXDir(0,pTarget);
 return(1);
}

protected func FxHoldObjectStop(pTarget,iEffectNumber)
{
 if(pTarget)pTarget->SetCategory(EffectVar(1,pTarget,iEffectNumber));
}
