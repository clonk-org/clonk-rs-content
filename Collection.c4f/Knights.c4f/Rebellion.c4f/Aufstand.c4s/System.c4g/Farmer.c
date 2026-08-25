/*-- Bauernclonk --*/

#strict

#appendto CLNK

private func IsFarmer()
{ // Farmer sind nur normale Clonks (CLNK) der Spieler im Dorf
  return( (GetID() == CLNK) && (Global(GetOwner()) == Position_Village) );
}

protected func Initialize() 
{
  _inherited();
  if(!IsFarmer())
    return(1);
  
  SetGraphics("", this(), _FRM);
  SetPhysical("Energy", GetPhysical("Energy") * 115 / 100, 2);   // Energie:  +15%
  SetPhysical("Dig", GetPhysical("Dig") * 110 / 100, 2);         // Graben:   +10%
  SetPhysical("Push", GetPhysical("Push") * 120 / 100, 2);       // Schieben: +20%
  SetPhysical("CanConstruct", 150, 2);                           // Bauen:    +50%
  SetPhysical("Fight", GetPhysical("Fight") * 125 / 100, 2);     // Kämpfen:  +25% (Mistgabel!)

  return(1);
}

