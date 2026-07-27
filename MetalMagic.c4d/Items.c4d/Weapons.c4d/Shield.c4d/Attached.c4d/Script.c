#strict 2

public func AttachTargetLost()
{
  ChangeDef(UnbuckledID() );
  if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
}

protected func UpdateTransferZone()
{
  if(GetID()!=SHIA) return;
  var pClonk = GetActionTarget();
  if(SetGraphics("Shield", pClonk, GetID(pClonk), KNIG_ShieldLayer, GFXOV_MODE_ExtraGraphics))
  {
    ChangeDef(UnbuckledID());
    if(!pClonk->WearShield(this, "Shield"))
      ChangeDef(SHIA);
  }

}
  
public func Unbuckle()
{
  ChangeDef(UnbuckledID() );
  if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  Sound("Connect");
  return 1;
}

public func UnbuckledID() { return SHIE; }

