/*-- Edelsteintruhe --*/

#strict 2

#include CHST

protected func RejectCollect(id idObj, object pObj)
  {
  if(idObj != GMSN) return 1;
  // Gegenstände nur annehmen, wenn die Truhe offen ist
  return GetAction() != "Open";
  }
 
func Collection(pObj)
{
 RemoveObject(pObj);
 Sound("Cash");
}
