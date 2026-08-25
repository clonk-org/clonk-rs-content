/*-- Selbstmordclonk --*/

#strict

#appendto CLNK

local pSuezBomb; // Clonk ist ein Selbstmordbomber
local pTrigger;  // Bombe wurde ausgelöst

/* Ist dies Selbstmordclonk? */

protected func IsSuezideClonk()
{
if (pSuezBomb) return(true);
else return(false);
}

/* selbstmörder Gürtel Kontext */

public func ContextTrigger(pCaller)
{
  [$CtxTrigger$|Image=DFLM|Condition=IsSuezideClonk]
  if (!pTrigger) Suezide();
  return(1);
}

public func BuckleBelt()
{
  SetGraphics("Attached",this(),_BLT,GFX_Overlay,GFXOV_MODE_ExtraGraphics);
  // Beschränkte Physicals ...
  SetPhysical("Jump", GetPhysical("Jump",0,this()) * 3 / 4, 2,this()); // Sprungkraft temporär vermindern
  SetPhysical("Walk", GetPhysical("Walk",0,this()) * 3 / 4, 2,this()); // Laufgeschwindigkeit temporär vermindern
  pSuezBomb=1;
}

protected func ControlDigDouble()
{
  if (IsSuezideClonk() && !pTrigger)
  {
  Suezide();
  }
  return(_inherited(...));
}

/* Fallen und Treffer */ 
protected func Hit3() 
{
  if (IsSuezideClonk() && !pTrigger) Suezide();
  return(_inherited(...));
}

protected func CatchBlow(int iLevel, object pBy) {
  // Bei mehr als 5 Schadenspunkten und die Bombe noch nicht gezündet wurde Explosion auslösen
  if (IsSuezideClonk() && iLevel > 5 && !pTrigger) Suezide();
  return(_inherited(...));
}

protected func Death(int iKilledBy)
{
  // Wenns ein Selbstmörder war dann wieder Clonk draus machen und Gürtel ablegen
  if(IsSuezideClonk())
  {
  ResetPhysical();
  SetGraphics(0,this(),0,1);
  CreateObject(_BLT,0,0,GetOwner());
  }
  return(_inherited(...));
}

protected func Suezide()
{
	pTrigger=1;		// Bombe wurde ausgelöst
	Sound("Scream");
  	CastObjects(DFLM, 20, 40);
	Explode(30,CreateObject(GUNP));
	Explode(30,CreateObject(GUNP));
	Explode(60);
}