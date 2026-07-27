#strict 2

public func AttachTargetLost()
{
  ChangeDef(UnbuckledID() );
  //if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
}
  
public func Unbuckle()
{
  ChangeDef(UnbuckledID() );
  //if(MMGetPlayerChurch(GetOwner())==scBloodchurch) SetGraphics("Dark");
  Sound("Connect");
  return 1;
}

public func UnbuckledID() { return SHIE; }

