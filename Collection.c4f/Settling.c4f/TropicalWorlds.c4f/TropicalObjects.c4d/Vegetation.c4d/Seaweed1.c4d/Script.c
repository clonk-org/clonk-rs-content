/*-- Seegras --*/

#strict
#include TREE

local iSeed;
local MySize;

private func ReproductionAreaSize() { return(80); }
private func ReproductionRate()     { return(22); }
private func MaxTreeCount()         { return(12); }

protected func Initialize()
{
  SetAction("Wiggle");
  return(1);
}
public func CheckInWater(){
  if(!InLiquid()) RemoveObject();
  return(1);
}

protected func Damage() {}

public func Construction() {
  SetDir(Random(4));
  SetPosition(GetX(),GetY()-5);	// baum-syndrom
}

public func Reproduction()
{
  // Ist noch Platz für einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
  if (ObjectCount(GetID(), iOffset, iOffset, iSize, iSize)<MaxTreeCount()) {
    // OK, hin damit
    iSeed=0;
    //Message("Test1",this());
   var pPlant = PlaceVegetation(GetID(this()), iOffset, iOffset, iSize, iSize, 10);
   pPlant->CheckInWater();
   return(pPlant);
  }
  // Kein Platz ;'(
  //Message("Test2",this());
  iSeed=0;
  return(0);
}

public func ContextChop(pClonk)		// per Kontextmenü NIX
{
  [$TxtChop$|Image=CXCP|Condition=IsStanding]
  AddCommand(pClonk, "Chop", this());
  return(1);
}

public func ChopDown()
{
 RemoveObject(this());
  return(1);
}

protected func Existing() {
  if(IsStanding()) return();
  // re-seed
  if(!Contained())
    if(!GetYDir())
      if(!GetXDir())
     SetCategory(32772);
}


protected func Seed()
{
  iSeed++;
  MySize++;
  if(iSeed==150){
  // Je nach Fortpflanzungsrate mehr oder weniger oft...
  if (!Random(ReproductionRate()))
    // ...reproduzieren
    Reproduction();
    
  }
  if(iSeed==155){iSeed=0;}
   if(MySize==15){
   DoCon(10);
   MySize=0;
   }
  if (GBackLiquid())
    if (GetAction() S= "Dry")
      return(StartWiggle());
  if (!GBackLiquid())
    if (GetAction() S= "Wiggle")
      return(StartDry());
  return(1);
}

private func StartWiggle()
{
  SetAction("Wiggle");
  SetPhase(Random(8));
}
  
private func StartDry()
{
  SetAction("Dry");
}
 
protected func Completion()
{
  StartDry();
  return(1);
}
