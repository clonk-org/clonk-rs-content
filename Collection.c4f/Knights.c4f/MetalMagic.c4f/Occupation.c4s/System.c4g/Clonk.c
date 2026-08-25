/*-- Neues Script --*/

#strict 2

#appendto CLNK

protected func Initialize()
{
 AddEffect("DamageCounter",this,20,100,0,GetID());
 return _inherited(); 
}

protected func Death(iBy)
{
 AddEffect("FadeOut",this,20,5,0,GetID());
 
 if(GetPlayerName(iBy))
 if(GetPlayerTeam(iBy) != GetPlayerTeam(GetOwner()))
 {
  Sound("Cash");
  DoWealth(25,iBy);
 }
 
 return _inherited();
}

protected func FxDamageCounterStart()
{
 return 1;
}

protected func FxDamageCounterDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
 if(iDmgEngy >0)return iDmgEngy;
 
 if(GetEnergy(pTarget)<=Abs(iDmgEngy)/1000)
 {
  //while(Contents(0,pTarget))
   //if((Contents(0,pTarget)->~IsMuskete())) RemoveObject(Contents(0,pTarget));
  
   for(var cnt=0;cnt<ContentsCount(0,pTarget);cnt++)
   {
    RemoveObject(Contents(cnt,pTarget));
   }
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
