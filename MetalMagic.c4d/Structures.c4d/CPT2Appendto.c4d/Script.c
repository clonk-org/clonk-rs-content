/*-- Appendto --*/

#strict 2


#appendto CPT2
#include CPT2
protected func Initialize()
{
  // Hintergrund-Wand erzeugen
  var pWall;
  pWall = CreateObject(CPW3, 0, +30, GetOwner());
  
  // An Hintergrund-Wand ausrichten
  SetPosition(pWall->GetX(), pWall->GetY() + 7);
  // Brustwehren erzeugen
  CreateObject(CPB1, -21, -27, GetOwner())->AttachTo(pWall, DIR_Left);
  CreateObject(CPB1, +22, -27, GetOwner())->AttachTo(pWall, DIR_Right);
  // Niedergang erzeugen
  CreateObject(CPTE, 0, -31, GetOwner());
  // Tür ist immer offen
  SetEntrance(1);
  // Turmspezifische Teile erzeugen
  InitializeTower();
  // Transferzone setzen
  UpdateTransferZone();
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
  
  SetObjectOrder(pWall,this,0);
  // Fertig
  return 1;
}
