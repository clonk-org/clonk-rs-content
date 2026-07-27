/*-- Unbrennbare Bäume --*/

#strict 2

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

public func Initialize()
{
  // Allen Bäumen Brandschutz geben
  for(var pTree in FindObjects(Find_Func("IsTree")))
    AddEffect("NoBurn", pTree, 500, 0, pTree);
  return 1;
}
