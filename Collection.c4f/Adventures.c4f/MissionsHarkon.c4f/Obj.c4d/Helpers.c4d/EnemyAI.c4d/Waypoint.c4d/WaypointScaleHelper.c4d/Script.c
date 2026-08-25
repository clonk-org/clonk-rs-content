/* Waypoint */

#strict 2

// Visualization

public func SetVisible(bool vis) {
  if(!vis) {
    SetAction("Idle");
    Message("", this());
		SetCategory(C4D_StaticBack);
  }
  else {
    SetAction("Vis");
		SetCategory(4194312);
    Message("@%d", this(), ObjectNumber());
  }
}

/* Intern */

protected func Initialize()
{
  SetObjectBlitMode(GFX_BLIT_Additive);
  if(WAYP_visibleWaypoints) { Message("@%d", this(), ObjectNumber()); SetAction("Vis");SetCategory(4194312|C4D_Foreground); }
}

func MouseSelection()
{
	FindObject(WPED)->Selection(this);
}