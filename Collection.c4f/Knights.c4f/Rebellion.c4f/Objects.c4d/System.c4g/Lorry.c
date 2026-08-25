/*-- Lore --*/

#strict

#appendto LORY

protected func ControlDig(object pClonk)
{
  // Holen-Menü öffnen
  SetCommand(pClonk, "Activate", 0, 0, 0, this());
  return(true);
}

