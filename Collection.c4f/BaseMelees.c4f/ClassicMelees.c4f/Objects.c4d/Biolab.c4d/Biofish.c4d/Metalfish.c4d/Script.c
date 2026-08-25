/*-- Metallfisch --*/

#strict
#include _MOF

private func TargetRange() {return(400);}  // Reichweite Gegner finden
private func AttackRange() {return(200);}  // Reichweite Angriff
private func ChargeLevel() {return(2);}    // Timercalls bis nächster Angriff

protected func Death()
{
  Sound("MetalHit*");
  CastPXS("Oil", 15, 20);
  CastObjects(_BLP, 5, 5, 0, 0); 
  RemoveObject();
  return(1);
}

private func Attack(object pObj)
{
  SetPhysical("Swim", GetPhysical("Swim") * 4 , 2);	// Angriffsgeschwindigkeit
  Schedule("ResetPhysical()", 30);			// Zeitlich begrenzt
  return(Attack2(pObj));
}

private func Attack2(object pObj)
{
  if(ObjectDistance(pObj) > 2) return(0);	// Nicht in Reichweite?
  Sound("MetalHit*");
  DoEnergy(-4,pObj);
  if(GetOCF(pObj)&OCF_CrewMember())
  Sound("Hurt*",0,pObj);
  if(!Random(3)) SetComDir(COMD_Stop(),pObj);
  return(1);
}