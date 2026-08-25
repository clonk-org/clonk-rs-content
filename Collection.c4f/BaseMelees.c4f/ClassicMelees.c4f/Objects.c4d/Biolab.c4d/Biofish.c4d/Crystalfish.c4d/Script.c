/*-- Kristallfisch --*/

#strict
#include _MOF

private func TargetRange() {return(500);} // Reichweite Gegner finden
private func AttackRange() {return(250);} // Reichweite Angriff
private func OnlyClonks()  {return(1);}   // Nur Clonks angreifen
private func ChargeLevel() {return(12);}  // Timercalls bis nächster Angriff

private func Attack(object pObj)
{
  // Blitz erzeugen
  var pCaster = this();
  var iAngle = Angle (GetX(), GetY(), GetX(pObj), GetY(pObj), 10);
  var obj = CreateObject(LGTS, -GetX(), -GetY(), GetOwner(pCaster));
  obj->Launch(GetX(pCaster)+GetVertex(0, VTX_X, pCaster), GetY(pCaster)+GetVertex(0, VTX_Y, pCaster), iAngle);
  Sound("Electric");
  if(!Random(3)) SetComDir(COMD_Stop(),pObj);
  // Energie vom Blitzschlag zurückgeben
  DoEnergy(12000, pCaster, 1); 
  return(1);
}

protected func Death()
{
  Sound("Crystal*");
  CastObjects (_BLX, 5, 5, 0, 0); 
  RemoveObject();
  return(1);
}