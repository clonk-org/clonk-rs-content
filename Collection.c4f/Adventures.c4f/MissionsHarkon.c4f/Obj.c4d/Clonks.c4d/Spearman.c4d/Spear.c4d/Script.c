/*-- Speer --*/

#strict 2

// Nicht ablegen lassen!
public func Departure(object pContainer)
{
  if(GetAlive(pContainer))
    return Enter(pContainer);
  else RemoveObject();
}

/* Werfen */
public func ControlThrow()
{
  // Träger ermitteln
  var pClonk = Contained();
  // Träger soll zustechen
  if (GetAction(pClonk) == "Walk") ObjectSetAction(pClonk, "Strike");
  else if (GetAction(pClonk) == "Jump") ObjectSetAction(pClonk, "StrikeJump");
  // Fertig
  return(1);
}