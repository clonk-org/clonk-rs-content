/*-- Neues Script --*/

#strict 2

#appendto CLNK

protected func Initialize()
{
 AddEffect("DamageCounter",this,20,100,0,GetID());
 return _inherited(); 
}

protected func MenuQueryCancel()
{
 // Charakterwahl nicht abbrechen
 if (GetMenu() == WIPF) return 1;
}

protected func Death(iBy)
{ 
 AddEffect("FadeOut",this,20,5,0,GetID());
 return _inherited();
}

protected func FxDamageCounterStart()
{
 return 1;
}

protected func FxDamageCounterDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
 if(iDmgEngy >0)return iDmgEngy;
 
 iDmgEngy*=5;
 
 if(GetEnergy(pTarget)<=Abs(iDmgEngy)/1000)
 {
  //while(Contents(0,pTarget))
   //if((Contents(0,pTarget)->~IsMuskete())) RemoveObject(Contents(0,pTarget));
  
  var helper=FindObject2(Find_ID(_HLP),Find_Owner(GetOwner(pTarget)));
  if(!helper)
  helper=CreateObject(_HLP,AbsX(10),AbsY(10),GetOwner(pTarget));
   for(var cnt=ContentsCount(0,pTarget);cnt>=0;cnt--)
   {
    //RemoveObject(Contents(cnt,pTarget));
    var obj=Contents(cnt,pTarget);
    if(GetID(obj) == GMSN)continue;
    Enter(helper,obj);
    
   }
 LockItemTrans(true);
 var pClonk=CreateObject(GetID(pTarget),AbsX(10),AbsY(10),-1);
 GrabObjectInfo(pTarget,pClonk);
 RemoveObject(pClonk);
 LockItemTrans(false);
 }
 
 return iDmgEngy;
}

protected func FxDamageCounterTimer()
{
 return 1;
}

protected func FxFadeOutTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(iEffectTime > 250){RemoveObject(pTarget);return(-1);}
 SetClrModulation(RGBa(255,255,255,iEffectTime),pTarget);
 
 return 1;
}
