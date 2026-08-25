/*-- Biolabor --*/

#strict

/* Nutzt die Grundfunktionalität der Werkstatt. */

#include WRKS
#include BAS5

/* Produkteigenschaften (überladen) */
public func ProductType () {return(C4D_Object());}
public func ProductCondition () {return("IsRefineryProduct");}
public func MaxDamage() {return(115);}

/* Produktion */

private func MenuProduction(pCaller) 
{
  // Menü erzeugen und mit Bauplänen des Spielers füllen
  CreateMenu(NFDY,pCaller,this(),1,"$TxtNoconstructionplans$");
  for(var i=0,idKnowledge; idKnowledge=GetPlrKnowledge(GetOwner(pCaller),0,i,ProductType ()); ++i)
    if (DefinitionCall (idKnowledge, ProductCondition ()))
      AddMenuItem("$TxtProductions$", "SelectProduction", idKnowledge, pCaller, 0, pCaller);
  return(1);
}

/* Türsteuerung */

private func SoundOpenDoor()
{
  Sound("SteelGate2");
}
  
private func SoundCloseDoor()
{
  Sound("SteelGate2");
}

/* Aktivität */

private func Smoking()
{
  if (Random(2)) CastObjects ( GAS_, 1, 0, +7, -27);
  CastObjects ( GAS_, 1, 0, -5, -28);
  InsertMaterial (Material("Acid"), 19, -16, Random(11)-5, -Random(25));
  InsertMaterial (Material("Acid"), 19, -16, Random(11)-5, -Random(25));
  Sound("Cook");
}

/* Aufnahme */

protected func Collection()
{
  return(Sound("Clonk"));
}

/* Kontext */

public func ContextConstruction(pCaller) 
{
  [$Production$|Image=_CH0|Condition=IsBuilt]
  return(MenuProduction(pCaller));
}

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if (GetDamage() > MaxDamage()) {
  CastPXS("Acid", 500, 20);
  CastObjects ( GAS_, 50, 5, 0, 0);
  // Zerstören
  Explode(50);
  }
}