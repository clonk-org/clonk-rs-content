/*-- Mühle --*/

#strict 2

protected func Initialize()
{
  SetEntrance(1);
  //Fundament manuell platzieren
  CreateObject(B_60,-12,55);
  return 1;
}
  
/* Kontext */

private func IsComplete()
{
  return GetCon() >= 100; 
}

func IsTurning() { return WildcardMatch(GetAction(),"Turn?"); } 
