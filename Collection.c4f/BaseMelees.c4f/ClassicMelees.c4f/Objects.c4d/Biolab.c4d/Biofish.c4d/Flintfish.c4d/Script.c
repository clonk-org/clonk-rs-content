/*-- Flintfisch --*/

#strict
#include _MOF

private func AttackRange() {return(15);}  // Reichweite Angriff
private func OnlyClonks()  {return(0);}   // Nur Clonks angreifen
private func TargetRange() {return(200);} // Reichweite Gegner finden

protected func Death()
{
  Explode(15);
  return(1);
}

private func Attack(object pObj)
{
  Explode(30);
  return(1);
}