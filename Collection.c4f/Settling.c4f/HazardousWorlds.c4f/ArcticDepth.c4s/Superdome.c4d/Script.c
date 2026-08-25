/*-- Superkuppel --*/

#strict

protected func Initialize()     // Initialisierung
{
  SetCategory(C4D_StaticBack());
  // Airlocks erzeugen wenn noch nicht da
  if(!FindObject(AIRL,-190,100))
    CreateConstruction(AIRL,-190,105,GetOwner(),100,1)->AIRL::SetLeft();
  if(!FindObject(AIRL,+190,100))
    CreateConstruction(AIRL,+190,105,GetOwner(),100,1)->AIRL::SetRight();

  // Wasser abpumpen
  SetAction("Pump");
}

protected func Damage()         // Schaden
{
  // Zerstörung bei zuviel Damage
  if(GetDamage()>250)
    if(GetCategory()&C4D_StaticBack())
      Destruction();
}

protected func Destruction()        // Zerstörung
{
  DoCon(-50);
  Sound("DomeDestruct");
  SetCategory(C4D_Structure());
}

protected func Transfer()       // StartCall von Pump
{
  // Wasser absaugen
  for(var i=0;i<30;++i)
    InsertMaterial(ExtractLiquid(0,RandomX(80,100)),-145,-45);
}

/* Callback vom Bausatz */

public func RejectConstruction(int x, int y, object clonk)
{
  var err = CheckPlacement(GetX(clonk) + x, GetY(clonk) + y);
  if(GetType(err) == C4V_String() )
  {
    Message(err, clonk);
    Sound("Error", 0, clonk);
    return(1);
  }
  else
  {
    return(0);
  }
}

private func CheckPlacement(int x, int y)
{
  // Nur im Wasser
  if(GetMaterial(x,y)!=Material("Water")) 
    return("$ErrNoWater$");
  
  // Tief genug
  if(y < 400)
    return("$ErrDepth$");
  // Sonst geht es   
  return(true);
}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(FSTW); }
