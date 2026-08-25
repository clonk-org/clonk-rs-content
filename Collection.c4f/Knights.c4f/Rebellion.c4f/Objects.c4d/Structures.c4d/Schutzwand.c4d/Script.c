/*-- Schutzwand --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
}

/* Ausrichtung */

protected func ControlRight(object pByObj)
{
  Sound("Lever1");
  SetDir(DIR_Right());
  SetSolidMask(67, 0, 26, 40, 7);
  return(1);
}

protected func ControlLeft(object pByObj)
{
  Sound("Lever1");
  SetDir(DIR_Left());
  SetSolidMask(52, 0, 26, 40, -7);
  return(1);
}

