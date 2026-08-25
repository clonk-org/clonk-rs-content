/* Aufsatteln */

#strict 2

func Activate(pCaster,pRealcaster)
{  
  if(!GetAction(pCaster)=="Walk")
  {
    Message("$MustStand$", pClonk);
    RemoveObject(); return;
  }
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  
  var pAnimal = FindObject2(Find_Owner(GetOwner(pClonk)), Find_Or(Find_ID(GRFN), Find_ID(SARN)), Find_Not(Find_Func("GetRider")));
  if(!pAnimal) pAnimal = FindObject2(Find_Or(Find_ID(GRFN), Find_ID(SARN)), Find_Not(Find_Func("GetRider")));
  if(!pAnimal) { Message("$NoAnimal$", pClonk); RemoveObject(); return; }
  var fx=GetX(pClonk)-GetX(), fy=GetY(pClonk)-GetY();
  pAnimal->ActivateEntrance(pClonk);
  var tx=GetX(pClonk)-GetX(), ty=GetY(pClonk)-GetY();
  DrawParticleLine("NoGravSpark", fx,fy, tx,ty, 10, 20, RGBa(127, 255, 255, 63), RGBa(127, 255, 255, 63));
  Sparkle(5, fx, fy);
  Sparkle(5, tx, ty);
  RemoveObject();
  return 1;
}

// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 10; }
