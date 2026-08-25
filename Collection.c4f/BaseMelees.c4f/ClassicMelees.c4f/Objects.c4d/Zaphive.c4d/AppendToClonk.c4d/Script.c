/*-- Zapmaster --*/

#strict

#appendto CLNK

local pMaster;
local UnbuckleContex;

/* Ist Zapmeister */

protected func IsZapmaster()
{
if (pMaster) return(true);
else return(false);
}

/* Zapmeister Haube Kontext */

public func BuckleCap()
{
  SetGraphics("Attached",this(),_ZMC,GFX_Overlay,GFXOV_MODE_ExtraGraphics);
  // Beschränkte Physicals ...
  SetPhysical("Jump", GetPhysical("Jump",0,this()) * 3 / 4, 2,this()); // Sprungkraft temporär vermindern
  SetPhysical("Walk", GetPhysical("Walk",0,this()) * 3 / 4, 2,this()); // Laufgeschwindigkeit temporär vermindern
  SetPhysical("CanScale", 0, 2,this()); 
  SetPhysical("CanHangle", 0, 2,this());
  UnbuckleContex=0;
  pMaster=1;
}

private func UnbuckleCap()
{
  ResetPhysical();
  SetGraphics(0,this(),0,1);
  Sound("Fuse");
  pMaster=0;
  if (UnbuckleContex) CreateObject(_ZMC);
}

protected func ControlDigDouble()
{
  if (IsZapmaster())
  {
  UnbuckleContex=1;
  UnbuckleCap();
  }
  return(_inherited(...));
}

/* Fallen */ 
protected func Hit3() 
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}

/* Steuerung append */ 

public func Feed()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}
/* Aktionen */

private func Fighting()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}
  
private func Chopping()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}
  
private func Building()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}

private func Processing()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}

private func Digging()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}

/* Ereignisse */
  
protected func CatchBlow()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}

/* Trinken */

public func Drink()
{
  if (IsZapmaster()) UnbuckleCap();
  return(_inherited(...));
}