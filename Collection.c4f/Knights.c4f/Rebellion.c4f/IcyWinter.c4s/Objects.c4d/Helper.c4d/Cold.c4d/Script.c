/*-- Kältewarnung --*/

#strict

protected func Initialize() { SetAlive(0); }

protected func AttachTargetLost() { return(RemoveObject()); }

