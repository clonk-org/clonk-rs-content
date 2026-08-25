/*-- Zugbrücke kann nicht bewegt werden --*/

#strict

#appendto CPT4

/* Torsteuerung */

protected func ControlUp()
{
  return(1);
}
  
protected func ControlDownSingle()
{
  return(1);
}

protected func CloseDrawbridge()
{
  var pBridge;
  if (pBridge = FindDrawbridge())
  	return(pBridge->ControlUp());
  return(0);
}
  
protected func OpenDrawbridge()
{
  var pBridge;
  if (pBridge = FindDrawbridge())
  	return(pBridge->ControlDown());
  return(0);
}

