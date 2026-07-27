/*-- gebrochener Holigonbarren --*/

#strict 2

// Für die Hilfsnachricht
protected func Entrance(pObj)
{
 if(GetOCF(pObj) & OCF_CrewMember && GetPlayerName(GetOwner(pObj)))MMShowHelpMessage("OnHoligonIngotCollect",GetOwner(pObj));
}

func Initialize() {
   
  return 1;
}

protected func Hit()
{
  Sound("Crystal*");
  return 1;
}

func IsAlchemContainer() { return true; }
func AlchemProcessTime() { return 200; }
