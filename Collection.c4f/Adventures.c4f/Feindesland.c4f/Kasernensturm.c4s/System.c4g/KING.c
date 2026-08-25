/*-- Neues Script --*/

#strict

#appendto KING

protected func Death(int iKiller, a,b,c,d,e)
{
  // Angelegtes Zeugs ablegen
  var obj;
  while(obj = FindAttachedItems(obj) ) ProtectedCall(obj, "AttachTargetLost");
  // Rüstung ablegen
  ArmorRemoved(); while (Contents()) Contents()->Exit();
  // Basisfunktion aufrufen
  return(_inherited(iKiller, a,b,c,d,e) );
  RemoveObject();
  return(1);
}
