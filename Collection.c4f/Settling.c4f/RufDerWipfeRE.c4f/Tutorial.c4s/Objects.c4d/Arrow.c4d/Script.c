/*-- Pfeil --*/

#strict

protected func Initialize() { SetAction("Turn"); }

global func SetArrow(iX,iY,fParralax,fUp) {
  RemoveAll(_AR1);
  var pArrow=CreateObject(_AR1,iX,iY,-1);
  if(fParralax) SetCategory(C4D_Object|33554432|2097152,pArrow);
  if(fUp)       pArrow->SetAction("TurnUp");
}
  
global func RemoveArrow() {
  RemoveAll(_AR1);
}
