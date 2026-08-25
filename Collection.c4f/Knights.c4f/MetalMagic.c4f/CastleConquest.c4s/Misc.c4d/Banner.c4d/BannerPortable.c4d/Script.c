/*-- Neues Objekt --*/

#strict 2
local pShown;
protected func RejectEntrance(obj)
{
 if(~GetOCF(obj) & OCF_CrewMember && GetID(obj) != FLGB)return 1;

}

protected func Entrance(obj)
{
 if(GetOCF(obj) & OCF_CrewMember)
 if(Global(GetOwner(obj))==1)
 { 
  Return();
  return 1;
  }
 
 CheckWinner(obj);
 
 if(pShown)RemoveShownFlag();
 ShowFlag(obj);
}

protected func Return()
{
   if(!ContentsCount(0,attackers_base))
    Enter(attackers_base,this);
  else 
    Enter(attackers_base2,this);
}

protected func Departure(obj)
{
 if(pShown)RemoveShownFlag(obj);
 if(!GetEffect("FlagRemove",this))AddEffect("FlagRemove",this,20,10,this);
}

protected func FxFlagRemoveStart()
{
 return 1;
}

protected func FxFlagRemoveTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(Contained())return -1;
 if(iEffectTime > 35*60*2){Return();return(-1);}
 return 1;
}

protected func ShowFlag(obj)
{
 pShown=CreateObject(POS1,0,0,-1);
 if(GetOCF(obj) & OCF_CrewMember)ObjectSetAction(pShown,"FlyCrew",obj);
 else ObjectSetAction(pShown,"HangUp",obj);
}

protected func RemoveShownFlag(obj)
{
 if(pShown)RemoveObject(pShown);
}

protected func CheckWinner(obj)
{
 if(obj != deffers_base)return 0;
 
 for(var cnt=0;cnt<GetPlayerCount();cnt++)
 {
  var player=GetPlayerByIndex(cnt);
  if(Global(player) != 1)continue;
  EliminatePlayer(player);
 }
}

public func HasEntered()
{
 RemoveShownFlag();
 Exit();
}

public func SetDown(obj)
{
 RemoveShownFlag();
 pShown=CreateObject(POS1,0,0,-1);
 ObjectSetAction(pShown,"HangDown",obj);
}
