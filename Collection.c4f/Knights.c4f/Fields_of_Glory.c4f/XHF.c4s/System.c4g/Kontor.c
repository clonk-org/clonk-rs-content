/*-- Einkaufen in Zelten --*/

#strict

#appendto CPOF
#appendto TENT

public func Collection2(pObj)
{
 // Bei JnR automatisch öffnen oder King
 if(GetPlrCoreJumpAndRunControl(GetController()))
 ContainedUp(pObj);
}

public func ContainedUp(pClonk,selection)
{
 if(GetID(pClonk) != KNIG && GetID(pClonk) != KING) return(1);
 CreateMenu(CPOF,pClonk,this(),C4MN_Extra_Value,"$NothingToBuy$",0,0,1);
 
 var obj=0;
 for(var cnt;obj=GetHomebaseMaterial(GetOwner(pClonk),0,cnt,28);cnt++)
 {
  AddMenuItem("$Buy$","Purchase",obj,pClonk,GetHomebaseMaterial(GetOwner(pClonk),obj),pClonk);
 }
 if(selection)
 SelectMenuItem(selection,pClonk);
}


public func Purchase(id,pClonk)
{
 var owner=GetOwner(pClonk);
 var selection=GetMenuSelection(pClonk);
 if(!GetHomebaseMaterial(owner,id))
 {
  PlayerMessage(owner,"$NotAviable$",this(),GetName(0,id));
  
  return(1);
 }
 
 if(GetWealth(owner) < GetValue(0,id))
 {
  PlayerMessage(owner,"$NotEnoughMoney$",this());
  
  return(1);
 }
 
 DoWealth(owner,-GetValue(0,id));
 DoHomebaseMaterial(owner,id,-1);
 
 var object=CreateObject(id,0,0,owner);
 Enter(this(),object);
 if(GetOCF(object) & OCF_Collectible()) Collect(object,pClonk);
 if(GetOCF(object) & OCF_Living()) MakeCrewMember(object, owner);
 
 ContainedUp(pClonk,selection);
 
 Sound("Cash"); 
}



