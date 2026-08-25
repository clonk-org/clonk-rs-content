/*-- Schwierigkeitsgrad wählen --*/

#strict

local clonk;

protected func ShowMenu(object pClonk)
{
  clonk = pClonk;
  CreateMenu(KNIG, clonk, this(), C4MN_Extra_None, "$Caption$", 0, 0, 0);
  AddMenuItem("$Easy$", "Selection", FISH, clonk, 0, 0, "$EasyDesc$");
  AddMenuItem("$Normal$", "Selection", WIPF, clonk, 0, 1, "$NormalDesc$");
  AddMenuItem("$Hard$", "Selection", MONS, clonk, 0, 2, "$HardDesc$");
  return(1);
}

protected func MenuQueryCancel(int iSelection, object pMenuObj)
{
  return(true);
}

private func Selection(id dummy, int difficulty)
{
  // iDifficulty: Statische Variable im Szenarienscript
  iDifficulty = difficulty;
  return(RemoveObject());
}
