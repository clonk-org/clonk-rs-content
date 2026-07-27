/*-- Neues Script --*/

#strict 2

#appendto WRKS

private func MenuProduction(pCaller) {
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(CXCN,pCaller,this,1,"$NoPlrKnowledge$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType ()); ++i)
  {
    if(ProductCondition())
      if(!DefinitionCall(idKnowledge, ProductCondition()))
        continue;
    if(idKnowledge->~RejectWorkshopConstruction(GetID()))
      continue;
    AddMenuItem("$Construction$: %s", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  }
  return 1;
}
