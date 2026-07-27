/*-- Schildständer --*/

#strict 2

public func RackedItem() { return "IsShield"; }
public func RackedItemCount() { return 4; }

protected func UpdateTransferZone()
{
  SetAction("Update");
}

protected func Update()
{
  var i, pObj;
  while(pObj = Contents(i++))
  {
    Ejection(pObj);
    Collection2(pObj);
  }
}

protected func RejectCollect(idObj, pObj)
  {
  // Verhindert Aufnahme aller nichtpassender Objekte
  if(!ObjectCall(pObj,RackedItem())) return 1;
  // Maximal 4 Stück
  if (ContentsCount() >= RackedItemCount())
  {
    Message( "$MsgRackFull$",this);
    return 1;
  }
  // Besitzer (Farbe) setzen
  if (GetOwner(pObj) != GetOwner()) SetOwner(GetOwner(pObj));
  return 0;
  }
  
protected func Collection2(pObj)
  {
  Sound("RackPut");
  var i=-1;
  while(Local(++i));
  Local(i) = pObj;
  SetGraphics(0,this, 0, i+1, GFXOV_MODE_Object, 0, 0, pObj);
  pObj->~EnterRack(i);
  return 1;
  }

protected func Ejection(pObj)
  {
  pObj->~LeaveRack();
  var i=-1;
  while(Local(++i)!=pObj && i<RackedItemCount());
  SetGraphics(0,this, 0, i+1, 0);
  Local(i) = 0;
  return 1;
  }

protected func ControlDig(pClonk)
  {
  [$CtrlTake$|Image=SHIE]
  // Gegenstand nehmen
  var pItem;
  for(var i=RackedItemCount()-1; i; i--)
    if(Local(i)) break;	  
  pItem = Local(i);
  if (!pItem) return Message("$MsgRackEmpty$", this);
  if (Collect(pItem, pClonk)) Sound("RackPut");
  return 1;
  }

