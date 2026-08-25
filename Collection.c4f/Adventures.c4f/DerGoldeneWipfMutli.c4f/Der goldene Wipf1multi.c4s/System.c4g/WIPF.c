/*-- Neues Script --*/

#strict
#appendto WIPF

protected func Activity()
{
  // Geräusche machen
  if (!Random(15)) Sound("Snuff*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Nur im Gehen oder Schwimmen
  if (GetAction() ne "Walk" && GetAction() ne "Swim") return();

  // Auftauchen, wenn der Atem ausgeht
  if (InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up())
   SetComDir(COMD_Up());

  // Brennenden Objekten ausweichen
  var pBurning;
  if(pBurning = FindObject(0, -!GetDir()*80,-30,80,35, OCF_OnFire()))
  {
   if(!Random(5) || !SpaceToFlee(pBurning))
   {
    if(GetDir()) SetComDir(COMD_Right());
    else SetComDir(COMD_Left());
    Jump();
   }
   else
   {
    if(GetComDir()==COMD_Left()) SetComDir(COMD_Right());
    else SetComDir(COMD_Left());
   }
  }

  // Nichts machen
  if (Random(2)) return();

  // Freigraben, falls er feststeckt
  if(Stuck()) return(DigFree());

  // Fortpflanzung
  //if (!Random(ReproductionRate())) 
 //   Reproduction();

  // Springen
  if (GetAction() eq "Walk")
   if (!Random(3)) return(Jump());

  // Hinsetzen
  if (GetAction() eq "Walk")
    if (!Random(3)) return(SetAction("Sit"));

  // Graben
  if (GetAction() eq "Walk")
    if (!Random(25)) return(StartDig());

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

public func RejectEntrance(object pNewCont, a,b,c) 
{
  return(0);
}
