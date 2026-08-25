/*-- Werkstadt --*/

#strict 2

#appendto WRKS
#appendto MWKS

public func NeedLevel() { if(GetID()==MWKS) return(3); return(1); }

public func CheckEmenyIntervall() { return 1; }

public func GetMaxEnergy() { return 400+300*(GetID()==MWKS); }

public func GetUpgradeValue() { if(GetID()==WRKS) return 10; }
public func UpgradeTo() { if(GetID()==WRKS) return MWKS; }


public func Upgraded()
{
  SetPosition(GetX(), GetY()-17);
}

protected func ContainedUp(pCaller) 
{
  [Image=CXCN]
  return MenuBuy(pCaller);
}

private func MenuProduction(pCaller) { MenuBuy(pCaller); }

private func MenuBuy(pCaller) {
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN,pCaller,this,C4MN_Extra_Value,"");
  if(GetID()==MWKS) AddMenuItem("$TxtConstructions$: %s", "SelectProduction", BGNT, pCaller, 0, pCaller);
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,C4D_Vehicle); ++i)
  {
//    if(ProductCondition())
//      if(!DefinitionCall(idKnowledge, ProductCondition()))
//        continue;
    if(iLevel >= idKnowledge->~NeedLevel())
      AddMenuItem("$TxtConstructions$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  }
  return 1;
}

public func SelectProduction(idProduct,pWorker,bSpecial2) {
  // Verfügbarkeit überprüfen
  if(GetWealth(GetOwner(pWorker)) < GetValue(0, idProduct))
  {
    PlayerMessage(GetOwner(pWorker),"$NotEnoughMoney$",this);
    return 1;
  }
  DoWealth(GetOwner(pWorker),-GetValue(0, idProduct));
  var pProduct = CreateContents(idProduct);
  SetOwner(GetOwner(pWorker), pProduct);
  if(idProduct==BGNT) { pProduct->~Summoned(pWorker); SetPlrViewRange(200, pProduct); }
  Smoking();
  
  // Sound
  Sound("Cash");
  Sound("Build1"); Sound("Build2");
  
  // Los geht's
  return 1;
}
