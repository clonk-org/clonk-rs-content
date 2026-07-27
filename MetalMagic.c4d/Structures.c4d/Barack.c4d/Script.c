/*-- Holzhütte --*/

#strict 2

#include DOOR
#include BAS4

local MenuClonks;

public func RejectConstruction(foo,bar,pClonk)
{
  var x=AbsX(GetX(pClonk));
  var y=AbsY(GetY(pClonk));
  if(!FindObject(PALS,x,y,0,0) && !FindObject(PAL2,x,y,0,0))
  {
    Message("$NoPalisade$",pClonk);
    return 1;
  } 
  return 0;
}

public func Initialize()
{
  MenuClonks=[];
 
  CreateObject(BRKD,0,0,GetOwner())->Activate(this,0);
 
  return _inherited();
}

public func ContextAscend(object caller)
{
  // Aufstieg auf die Palisade
  [$Ascend$|Image=MASC|Condition=IsDoorOpen]
  Ascend(MASC, caller);
}

// Statusfunktionen für die Klappe
public func IsDoorOpen() { if(GetAction(FindObject(BRKD,0,0,0,0,0,0,this)) == "Open") return 1; }
public func IsDoorClosed() { return !IsDoorOpen(); }

public func ContextOpenDoor(object caller)
{
  // Das Tor geht auf...
  [$OpenDoor$|Image=BRKD|Condition=IsDoorClosed]
  Switch();
}

public func ContextCloseDoor(object caller)
{
  // ... und wieder zu
  [$CloseDoor$|Image=BRKD|Condition=IsDoorOpen]
  Switch();
}

public func ContainedLeft(pObj)
{
  CreateMenu(BRCK,pObj,this,0,0,0,0,1);
 
  AddMenuItem("$Door$","Switch",BRKD,pObj);
  AddMenuItem("$Ascend$","Ascend",MASC,pObj,0,pObj);
 
  //Log("Vorher: %d",GetLength(MenuClonks));
  if(GetArrayItemPosition(pObj,MenuClonks) == -1)PushBack(pObj,MenuClonks);
  //Log("Nachher: %d",GetLength(MenuClonks));
  if(!GetEffect("CheckMenu",pObj))AddEffect("CheckMenu",pObj,20,1,this,0,GetLength(MenuClonks));
  var id=0;
  var checked=[];
  var obj;
  for(var cnt=0;obj=Contents(cnt,this);cnt++)
  {
    id=GetID(obj);
    if(cnt>ContentsCount())break;
    if(GetArrayItemPosition(id,checked) != -1)continue;
  
    if(GetOCF(obj)&OCF_CrewMember)continue;
    PushBack(id,checked);
    AddMenuItem("$Activate$","ActivateItem",id,pObj,ContentsCount(id,this),pObj);
  }
  return 1;
}


protected func ActivateItem(iItem,pClonk,bSpecial2)
{
 if(!ContentsCount(iItem))return;
 
 var x=GetDefCoreVal("Entrance","DefCore",0,0)+GetDefCoreVal("Entrance","DefCore",0,2)/2;
 var y=GetDefCoreVal("Entrance","DefCore",0,1)+GetDefCoreVal("Entrance","DefCore",0,3);
 

 
 if(bSpecial2)
 {for(var obj;obj=FindObject(iItem,0,0,0,0,0,0,0,this,obj);)
 SetCommand(obj,"Exit");//Exit(obj,x,y);
 
 //SelectMenuItem(GetMenuSelection(pClonk)-1,pClonk);
 }
 if(!bSpecial2)
 SetCommand(FindContents(iItem),"Exit");//Exit(FindContents(iItem),x,y);
 
}

protected func Switch()
{
 var hatch=FindObject(BRKD,0,0,0,0,0,0,this);
 hatch->Switch();
}

protected func Ascend(food,pCaller)
{
 var hatch=FindObject(BRKD,0,0,0,0,0,0,this);
 if(GetAction(hatch) == "Close") return Message("$HatchClosed$",this);
 
 Exit(pCaller,-8,-8-10,0,0,0,0);
 
 CloseMenu(pCaller);
 return 1;
}
protected func RefreshMenus(iAdd)
{
 for(var clnk in MenuClonks)
 {
  var sel=GetMenuSelection(clnk);
  CloseMenu(clnk);
  ContainedLeft(clnk);
  if(sel == 0)iAdd=0;
  SelectMenuItem(sel+iAdd,clnk);
 }
}

protected func FxCheckMenuStart(pTarget,iEffectNumber,iTemp,val1)
{
 EffectVar(1,pTarget,iEffectNumber)=val1;
 return 1;
}

protected func FxCheckMenuTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(!this)return -1;
 if(GetMenu(pTarget)!=BRCK)
 {
  //if(MenuClonks[EffectVar(1,pTarget,iEffectNumber)] == pTarget)
  DeleteArrayItem(GetArrayItemPosition(pTarget,MenuClonks),MenuClonks);
  return -1;
 }
 
 return 1;
}

protected func Ejection()
{
 RefreshMenus();
 return _inherited(Par(),Par(1),Par(2));
}

protected func Collection2()
{
 var par1=1;
 if(ContentsCount(GetID(Par()))>1)par1=0;
 
 RefreshMenus(par1);
 return _inherited(Par(),Par(1),Par(2));
}
