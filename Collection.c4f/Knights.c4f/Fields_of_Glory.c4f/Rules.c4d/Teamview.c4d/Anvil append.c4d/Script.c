#strict
#appendto ANVL

private func Forging() {
  if(FindObject(RTVW)) SetPlrViewRange(10);
  // Noch nicht fertig
  if (GetActTime() < 150) return(0);
  
  var pProduct = ComposeContents(product);
  // Objekt auswerfen
  if(pProduct)
    Exit(pProduct);
  // Aktion beenden und Clonk auswerfen
  ProductionCancel();
  return(1);
}


private func ProductionCancel() {
  // Arbeit abbrechen
  if (GetAction() eq "Forge") SetAction("Idle");
  // Clonk(s) auswerfen
  var clnk;
  while (clnk = FindCrewContents()) Exit(clnk,0,10);
  // Fertig
  if(FindObject(RTVW)) SetPlrViewRange(0);
  return(1);
}
