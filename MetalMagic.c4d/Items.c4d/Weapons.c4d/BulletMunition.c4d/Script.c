/*-- Kugelpaket --*/

#strict 2

#include ARWP

/* Paketeigenschaften */

public func UnpackTo() { return BLLT; }
public func IsBulletPack() { return 1; }
public func MaxPackCount() { return 10; }
public func IsAnvilProduct() { return 1; }

public func IsArrowPack() { return 0; }

public func CarryLimit() { return 1; }

// overload
public func JoinPack(pClonk)
{
  if(GetEffect("NoEntrancePackaging", pClonk)) return false;
  // In vorhandene Pfeilpakete integrieren
  var obj, iChange, iAmount;
  // Alle gleichen Pakete im Clonk durchsuchen
  for(obj in FindObjects(Find_Exclude(this()), Find_Container(pClonk), Find_ID(GetID()))) 
  {
    iAmount = PackCount();
    // Wie viel Platz ist im neuen Paket?
    iChange = MaxPackCount() - obj->PackCount();
    // Wir können aber höchstens so viele abgeben wie wir haben
    if(iChange>iAmount) iChange = iAmount;
    // Pfeile abgeben
    obj->DoPackCount(iChange);
    // Wenn das Paket leer ist aufhören
    if(iAmount-iChange<=0) return(RemoveObject());
    DoPackCount(-iChange);
  }
  return;
}

/* Manuelle Zerlegung */

protected func Activate(pClonk)
{
  [$TxtUnpack$]
  var amount=MaxPackCount()-LocalN("iUsedItems");
  if(amount<=5)return;
  AddEffect("NoEntrancePackaging", pClonk, 1, 1);
  var other=CreateContents(GetID(),Contained());
  LocalN("iUsedItems",other)=MaxPackCount()-amount/2;
  var amount2=MaxPackCount()-LocalN("iUsedItems",other);
  amount=amount-amount2;
  LocalN("iUsedItems")=MaxPackCount()-amount;
  this->UpdatePicture();
  other->UpdatePicture();
}

public func SplitUpPack(pClonk)
{
  if(GetEffect("NoSplittingUp",this))return 0;
  // Aufsplitten
  var obj, iBulletCount;
  // Alle gleichen Pakete im Clonk durchsuchen
  for(obj in FindObjects(Find_Exclude(this), Find_Container(pClonk))) 
  {
    if(obj->~IsBulletPack())
    {
     iBulletCount+=obj->~MaxPackCount()-LocalN("iUsedItems",obj);
    }
    else
    if(obj->~IsBullet())
    {
     iBulletCount++;
    }
  }
  
  if(iBulletCount)
  {
   var j=0,sCall;
   var maxCount=0;
   while(sCall=pClonk->~GetMaxSpecialCount(j++,0))
   {
    if(sCall != "IsBullet")continue;
    maxCount=pClonk->~GetMaxSpecialCount(j-1,1);
    break;
   }
   
   if(maxCount)
   {
    var amount=MaxPackCount()-LocalN("iUsedItems",this);
    var wanted=maxCount-iBulletCount;
    if(wanted > amount)wanted=amount;
    if(!wanted)return 0;
    var other=CreateObject(GetID(),0,0,GetOwner());
    LocalN("iUsedItems",other)=other->MaxPackCount()-wanted;
    other->UpdatePicture();
    other->UpdateMass();
    AddEffect("NoSplittingUp",other,1,1,0,BLTP);
    if(Collect(other,pClonk))
    {
     LocalN("iUsedItems",this)+=wanted;
     this->UpdatePicture();
     this->UpdateMass();
     if(PackCount() <= 0)RemoveObject();
     return 1;
    }
    else
    {
     //Message("FAILEZ! wanted: %d|amount: %d|Other Used: %d",pClonk,wanted,amount,other->MaxPackCount()-wanted);
     RemoveObject(other);
     return 0;
    }
   }
  }
  return;
}

func FxNoSplittingUpStart()
{
 return 1;
}

func FxNoSplittingUpTimer()
{
 return -1;
}
