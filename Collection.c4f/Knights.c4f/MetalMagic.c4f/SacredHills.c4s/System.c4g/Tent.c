/*-- Einkaufen in Zelten --*/

#strict 2

#appendto TENT

public func ContextBuy(object caller)
{
  // Kaufen
  [$CtxtBuy$|Image=FLAG]
  ContainedUp(caller);
}

protected func Initialize()
{
  AddEffect("NoDamage",this,20,0,0,GetID());
  return _inherited();
}

public func ControlDigDouble()
{
 return 0;
}

public func ContainedUp(pClonk,selection)
{
 CreateMenu(TENT,pClonk,this,C4MN_Extra_Value,"$NothingToBuy$",0,0,1);
 
 var obj=0;
 for(var cnt;obj=GetHomebaseMaterial(GetOwner(pClonk),0,cnt,16);cnt++)
 {
  AddMenuItem("$Buy$","Purchase",obj,pClonk,GetHomebaseMaterial(GetOwner(pClonk),obj),pClonk);
 }
 if(selection)
 SelectMenuItem(selection,pClonk);
}


public func Purchase(idID,pClonk)
{
 var owner=GetOwner(pClonk);
 var selection=GetMenuSelection(pClonk);
 if(!GetHomebaseMaterial(owner,idID))
 {
  PlayerMessage(owner,"$NotAviable$",this,GetName(0,idID));
  
  return 1;
 }
 
 if(GetWealth(owner) < GetValue(0,idID))
 {
  PlayerMessage(owner,"$NotEnoughMoney$",this);
  
  return 1;
 }
 
 DoWealth(owner,-GetValue(0,idID));
 DoHomebaseMaterial(owner,idID,-1);
 
 var object=CreateObject(idID,0,0,owner);
 if(!Collect(object,pClonk))
 Enter(this,object);
 
 ContainedUp(pClonk,selection);
 
 Sound("Cash"); 
}


protected func FxNoDamageTimer()
{
 return 1;
}

protected func FxNoDamageDamage()
{
 return 0;
}





