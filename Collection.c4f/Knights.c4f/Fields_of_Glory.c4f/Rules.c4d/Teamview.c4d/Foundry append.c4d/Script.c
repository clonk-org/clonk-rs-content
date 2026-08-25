#strict
#appendto FNDR

private func Burning()
{
  if(FindObject(RTVW)) SetPlrViewRange(10);
  // Rauch
  Smoke(+23,Random(3)-24,Random(5)+8);
  // Noch nicht fertig
  if (GetActTime()<150) return(1);
  // Fertig
  SetAction("Idle");
  // Erz suchen
  var pOre;
  if (!(pOre=FindContents(ORE1))) return(1);
  // Erz verbrauchen
  RemoveObject(pOre);
  // Metall auswerfen
  Exit(CreateContents(METL), -27,+13,0, -1);
  // Sound
  Sound("Pshshsh");
  if(FindObject(RTVW)) SetPlrViewRange(0);
  // Fertig
  return(1);
}