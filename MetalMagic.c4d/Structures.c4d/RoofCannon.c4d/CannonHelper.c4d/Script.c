/*-- Hilfsobjekt --*/

#strict 2
#include CTW0

local collection, scheduler;
local iMaxRotR, iMaxRotL;
protected func AttachTargetLost() { return LooseTarget(); }

protected func MaxRotR() { if(!iMaxRotR) return( 110); return(iMaxRotR); }
protected func MaxRotL() { if(!iMaxRotL) return(-110); return(iMaxRotL); }

public func SetMaxRot(iR, iL)
{
  iMaxRotR = iR;
  iMaxRotL = iL;
}

public func LooseTarget()
{
  if(cannon)
    ReleaseCannon();

  SetPosition(GetX(),GetY()+10);
  RemoveObject(this,1);
  return 1;
}

public func Check4Target()
{
  scheduler++;
  if(OnFire(GetActionTarget()))LooseTarget();
  if(!GetActionTarget())LooseTarget();

  if(scheduler<4){ScheduleCall(this,"Check4Target",1,0);return(1);}
  scheduler=0;
  return;
}

protected func Initialize()
{
collection=1;
SetEntrance(1);
ScheduleCall(this,"TimeInitialize",1,0);
return _inherited();
}
protected func TimeInitialize()
{
if(basement)RemoveObject(basement);
}

//Funktionen, die überladen werden müssen
//Das connect wegen Pfeil
public func ConnectCannon(object pCannon)
{
  // Wir rufen die Connect-Funktion der Kanone auf. Somit kann die Kanone
  // eigene Dinge tun um sich anzubauen.
  pCannon->Connect(this );
  SetR(45, pCannon);
  if(GetX() > LandscapeWidth() / 2) SetR(-45, pCannon);
  
  // neue Kategorie für die Kanone um sie in den Hintergrund zu kriegen
  SetCategory(2,pCannon);
  SetObjectOrder(pCannon);

  // Die neue Kanone über enthaltene Objekte informieren
  var i = ContentsCount() - 1, obj;
  while(obj = Contents(--i) )
    if(!obj->~IsCannon() )
      pCannon->~ComEnter(obj);
  if(!FindObject(RFCA,0,0,0,0,0,"Be",pCannon))CreateObject(RFCA,0,0,-1)->InitializeToObject(pCannon);
  //ATTACH Vertex setzen
  SetVertex (0, 1,10, pCannon, 2); 

    //Wir brauchen Effekte!
    AddEffect("CannonCheck", pCannon, 20, 1, this,0);
    SetR(0,pCannon);
    
   /* //arrr  nope
    if(GetID(pCannon)==CTW6){
    if(GetVertexNum(pCannon)==1)AddVertex(0,-6,pCannon);
    SetVertex(1,1,-6,pCannon);
    SetVertex(1,0,0,pCannon);
    SetVertex(1,2,30,pCannon);
    }*/
  cannon = pCannon;
  Sound("Connect");
}

//Lösen wegen verteices
// Löst die Kanone vom Geschützturm
public func ReleaseCannon()
{
  if(!cannon) return 0;
  Sound("Connect");
   
  SetVertex(0,1,GetDefCoreVal("VertexY","DefCore",GetID(cannon),0),cannon,2);
  cannon->ComStopDouble();
  SetVertex (0, 1, GetDefHeight(GetID(cannon))/2 , cannon, 2);
  //SetVertex(0,1,GetDefCoreVal("VertexY","DefCore",GetID(cannon),0),cannon,2);
  //SetVertex (1, 0, GetDefWidth(GetID(cannon))/2 , cannon, 1);
  
  //SetVertex (2, 0, -GetDefWidth(GetID(cannon))/2 , cannon, 1);
  Enter(GetActionTarget(),cannon);
  // Kategorie für die Kanone wiederherstellen
  SetCategory(GetDefCategory(GetID(cannon)),cannon);
  cannon = 0;
  
  // Neue Kanonen suchen
  var i, obj;
  while(obj = Contents(i++) )
  {
    if(obj->~IsCannon() )
    {
      ConnectCannon(obj);
      break;
    }
  }
  return 1;
}

/* Abfackeln */

protected func Incineration()
{
  if(cannon) ReleaseCannon();
  ChangeDef(CTW1);
  return _inherited() ;
}

protected func IncinerationEx()
{
  if(cannon) ReleaseCannon();
  ChangeDef(CTW1);
  return _inherited() ;
}

//Steuerung, weil keinen Eingang
protected func ContainedDown(object pObj)
{
  /* Kanone anhalten und den Clonk noch rauslassen */
  ContainedUp(pObj);
  
  pObj->Enter(GetActionTarget());
  GetActionTarget()->ContainedLeft(pObj);
  return 1;
}

//Muss auch überladen werden
protected func ContainedLeft(object pObj,p2,p3,dont)
{
  [$TxtRotatecannontothelef$]
  if(!cannon){return 0;}
  Sound("Click");
  if(GetR(cannon)<= MaxRotL()+4)return 1;
  
  return cannon->ComLeft(pObj) ;
}

protected func ContainedRight(object pObj)
{
  [$TxtRotatecannontotherig$]
  if(!cannon) return 0;
  Sound("Click");
  if(GetR(cannon)>= MaxRotR()-4)return 1;
  
  return cannon->ComRight(pObj) ;
}

//Imitiere Auslagerungsmenü und so
/*protected func ContainedThrow(object pObj,p2,p3,fromthis,dont)
{

SetEntrance(1);
//if(!dont){this->ContainedThrow(pObj,0,0,0,1);return(1);}

return;
if(GetMenu(pObj)==GetID())return 1;
if(Contents(0,pObj) && !fromthis)return 0;
CreateMenu(GetID(),pObj,0,0,"Kanonenturm ist leer",0,0,0,1);
for(var i;i<ContentsCount();i++)
{
var stop=0;
for(var v=0;true;v++)
{
if(Var(v)==GetID(Contents(i))){stop=1;break;}
if(Var(v))continue;
Var(v)=GetID(Contents(i));
break;
}
if(stop)continue;
if(~GetOCF(Contents(i)) & OCF_CrewMember)
AddMenuItem("%s aktivierenZ","TransferToTower",GetID(Contents(i)),pObj,ContentsCount(GetID(Contents(i))),pObj,0,0,Contents(i));
}

return 1;
}*/

//Transferieren
public func TransferToTower(ID,obj,all)
{
var o;
if(all)
while(o=FindContents(ID))
{
Exit(o);
Enter(GetActionTarget(),o);
}
else
{
//var obj=FindContents(ID);
Exit(obj);
Enter(GetActionTarget(),obj);
}
GetActionTarget()->ContainedLeft(this);
//this->ContainedThrow(clonk,0,0,1);
}
//nochmal was
protected func Ejection(obj)
{
//if(!Contained(obj))TransferToTower(0,obj);
}
/*protected func RejectCollect(par1,obj)
{
if(Contained(obj))return 0;
return collection;
}*/

//Jetzt ein Effekt
//Dieser muss da sein, damit die Kanone sich nur bis zu einem bestimmten Winkel drehen kann.


protected func FxCannonCheckStart(pTarget, iEffectNumber)
  {

  EffectVar(0, pTarget, iEffectNumber) = GetID(pTarget);

  // Fertig
  return 1;
  }


protected func FxCannonCheckTimer(pTarget, iEffectNumber)
  {
 
//Ncoh was extra machen
  if(EffectVar(0, pTarget, iEffectNumber)!=O__O)//Zum Filtern von bestimmten Geschützen ^^
  {
  if(!Inside(GetR(pTarget), MaxRotL(), MaxRotR()) && GetRDir(pTarget)){pTarget->ComStop();Sound("CannonStop");}
  

 
 
 
 if(Inside(GetR(pTarget),-100,MaxRotL()-1))SetR(MaxRotL(),pTarget);
  if(Inside(GetR(pTarget),MaxRotR()+1,100))SetR(MaxRotR(),pTarget);
  
  
  }
  else
  {
  
  //SetR(BoundBy(GetR(pTarget)+80,0,160)-80,pTarget);
  
  }
  //Löschen?
  if(GetID(pTarget)!=EffectVar(0,pTarget,iEffectNumber)
  || !pTarget)return -1;
  // Fertig nicht löschen
  return 1;
  }
  

