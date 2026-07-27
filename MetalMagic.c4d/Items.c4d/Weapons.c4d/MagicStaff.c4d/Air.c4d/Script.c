/*--- Stab der Luft  ---*/

#strict 2

local aSpells;
local aCombos;
local iColor;

public func Initialize() {
  return 1;
}

public func SetSpells(aNewSpells, aNewCombos)
{
  // Wenn keine Sprüche angegeben sind, werden alle verwendet,
  // die die StabID bei GetSpellStaff() zurückgeben
  // ansonsten NUR die die in aSpells sind, unabhängig von ihrerm GetSpellStaff()-Wert
  // ist aCombos angegeben, so werden die Combotasten der Sprüche in aSpells für den Stab überladen
  // dabei gehören jeweils Spruch und Combo mit dem gleichen Arrayindex zusammen
  aSpells = aNewSpells;
  aCombos = aNewCombos;
  return 1;
}

public func SetColor(iRgb)
{
  // Hiermit kann man die Inventarfarbe überladen
  iColor = iRgb;
}
  
public func Departure(object pCaller)
{
  if(GetAction(pCaller) != "Throw" && GetAction(pCaller) != "RideThrow" )return;
  var angle=-5;
  if(GetDir(pCaller)==DIR_Left)angle=-175;
  if(GetMagicEnergy(pCaller)>7)
  {
    DoMagicEnergy(-7,pCaller);
    AddEffect("StaffFlight",this,20,2,this,0,pCaller,angle);
  }
}

public func DepatureX(object pCaller)
{
  var angle= 25;
  if(GetXDir()<0)angle=-205;
  if(GetMagicEnergy(pCaller)>7)
  {
    DoMagicEnergy(-7,pCaller);
    AddEffect("StaffFlight",this,20,2,this,0,pCaller,angle);
  }
}

protected func Hit()
{
  if(GetEffect("StaffFlight",this))
  {
    SetXDir(GetXDir()/-2);SetYDir(GetYDir()/-2);SetRDir(Random(180)+1);
    EffectVar(3,this,GetEffect("StaffFlight",this))=1;
  }
}
  
protected func FxStaffFlightStart(object pTarget, int iEffectNumber, int iTemp, par1,angle)
{
  EffectVar(0,pTarget,iEffectNumber)=par1;
  EffectVar(1,pTarget,iEffectNumber)=angle;
  EffectVar(3,pTarget,iEffectNumber)=0;
  SetXDir(Cos(angle,50));
  SetYDir(Sin(angle,50));
  
  SetRDir(90);
}
  
protected func FxStaffFlightTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  if(!EffectVar(0,pTarget,iEffectNumber)) return -1;
  if(Contained(pTarget)) return -1;
  if(EffectVar(0,pTarget,iEffectNumber)->~HasStaff()) return -1;
  if(!GetRDir(pTarget))SetRDir(Random(180),pTarget);
  if(Abs(GetRDir(pTarget))<90)SetRDir(GetRDir(pTarget)+5,pTarget);
   
  if(!GetAlive(EffectVar(0,pTarget,iEffectNumber)))return -1;
   
  if(ObjectDistance(this,EffectVar(0,pTarget,iEffectNumber))<GetDefCoreVal("Height","DefCore",GetID(EffectVar(0,pTarget,iEffectNumber)))/2 || iEffectTime>36*10)
  {
    Collect(this,EffectVar(0,pTarget,iEffectNumber));
    return 0;
  }
   
  var angle=Angle(GetX(),GetY(),GetX(EffectVar(0,pTarget,iEffectNumber)),GetY(EffectVar(0,pTarget,iEffectNumber)));
   
  if(iEffectTime<20)
  {
    SetXDir(Cos(EffectVar(1,pTarget,iEffectNumber),40));
    SetYDir(Sin(EffectVar(1,pTarget,iEffectNumber),40));
    return 0;
  }
 
  SetXDir(BoundBy(GetX(EffectVar(0,pTarget,iEffectNumber))-GetX(),GetXDir()-10,GetXDir()+10));
  if(iEffectTime>35 || EffectVar(3,pTarget,iEffectNumber))SetYDir(BoundBy(GetY(EffectVar(0,pTarget,iEffectNumber))-GetY(),GetYDir()-10,GetYDir()+10));
  else SetYDir(-25);
   
  if(Abs(GetXDir())<3)SetXDir(Cos(angle,10));
  if(Abs(GetYDir())<3)SetYDir(Sin(angle,10));
   
  SetXDir(BoundBy(GetXDir(),-35,35));
  SetYDir(BoundBy(GetYDir(),-35,35));
   
  return 1;
}

protected func RejectEntrance(pNewContainer)
{
  if(GetEffect("StaffFlight", this))
  {
    if(pNewContainer != EffectVar(0,this,GetEffect("StaffFlight", this)))
      if(GetID(pNewContainer)!=MGST) return 1;
  }
}

public func GetSpell(iIndex)
{
  if(!aSpells)
  {
    aSpells = [];
    var idSpell;
    var i = 0;
    while(idSpell=GetDefinition(i++,1))
    {
      if(idSpell->~GetSpellStaff() == GetID())
        aSpells[GetLength(aSpells)] = idSpell;
    }
    // Standartsortierung für's Menü
    var aSpellsSort = [];
    var idSmallest, j;
    while(GetLength(aSpells))
    {
      idSmallest = 0;
      for(idSpell in aSpells)
    if(SpellSort(idSpell, idSmallest))
      idSmallest = idSpell;
      DeleteArrayItem(GetArrayItemPosition(idSmallest, aSpells), aSpells);
      PushBack(idSmallest, aSpellsSort);
    }
    aSpells = aSpellsSort;
    aCombos = [];
    for(idSpell in aSpells)
      aCombos[GetLength(aCombos)] = idSpell->~GetSpellStaffCombo();
  }
  return aSpells[iIndex];
}

private func SpellSort(idSpell, idSmallest)
{
  if(!idSmallest) return 1;
  // Identische werden nicht sortiert, weil Endlosschleifen doof sind
  if(idSpell->~GetSpellStaffCombo()==idSmallest->~GetSpellStaffCombo()) return 0;
  var aCombo = [2,5,3,1,6,4];
  var i;
  var iPos1, iPos2;
  while(1)
  {
    iPos1 = GetArrayItemPosition(GetChar(idSpell->~GetSpellStaffCombo(), i)-GetChar("0",0),aCombo);
    iPos2 = GetArrayItemPosition(GetChar(idSmallest->~GetSpellStaffCombo(), i)-GetChar("0",0),aCombo);
    i++;
    if(iPos1==iPos2) continue;
    if(iPos1<iPos2)
      return 1;
    return 0;
  }
}

public func GetSpellCombo(iIndex)
{
  if(!aCombos)
    return;
  return aCombos[iIndex];
}

// Routineabfragen für Herstellung
public func IsWizardTowerProduct() { return 1; }

public func IsStaff()   { return 1; }

// Gibt die Farbe an, die der Inventareffekt vom Stab bekommen soll
public func GetStaffColor() { if(iColor) return(iColor); return(GetMyStaffColor()); }

private func GetMyStaffColor() { return RGBa(150,150, 150, 128); }

public func GetClassCombo() { return "7"; }
