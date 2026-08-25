/*-- Buch --*/

#strict 2
  
public func Entrance(pObj)
{
	if(pObj->~GetClonk()) pObj = pObj->~GetClonk();
	pObj->~DoShow(200);
}
  
protected func Activate(object pPlayer)
  {
  // Nachricht
  Message("$MsgRead$", pPlayer);
  return;
  }

func IsQuestItem() { return 1; }