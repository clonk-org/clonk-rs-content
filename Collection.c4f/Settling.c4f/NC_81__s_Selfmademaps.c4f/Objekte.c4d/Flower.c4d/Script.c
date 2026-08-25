/*-- Blume --*/

#strict

protected func Initialize() 
{
  SetAction("Standard");
  var phase = Random(8);
  SetPhase(phase);
  ScriptGo(1);
}

protected func Damage() 
{
  if (GetDamage() < 5) return(0);
  CastObjects(ZAP1, 18, 15, 0, -5);
  RemoveObject();
  return(1);
}

protected func Zaps()
{
  if (Random(100)) return(1);
  CreateObject(ZAP1);
  return(1);
}
