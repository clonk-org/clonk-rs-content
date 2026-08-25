/*-- Der Clonk --*/

#strict

#appendto CLNK

protected func Initialize()
{
  // Sollen klettern können (wichtig für Film)
  SetPhysical("CanScale", 1, PHYS_Temporary, this());
  return(_inherited());
}

protected func Recruitment(a,b,c,d,e,f,g,h,i,j)
{
  // Sichtbereich verkleinern
  SetPlrViewRange(300);
  // Alchemiebeutel erzeugen
  CreateObject(ALC_, 10, 10, GetOwner())->Activate(this());
  // Wir wollen kein "Alchemiebeutel angelegt"!
  Message(" ", this());
  // Überladene Funktion aufrufen
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

