/*-- Eisblock --*/

#strict

local alpha;

// Startet den Schmelzvorgang
public func Destruct()
{
  // Scriptcounter setzen
  if(ScriptCounter() < 3400) 
    goto(3400);

  return(alpha+=1);
}

private func Init()
{
  var pClonk = GetActionTarget();
  pClonk->SetAction("Idle");
  pClonk->SetObjectLayer(this());
  SetObjectLayer(this());
}

private func NoAction()
{
  var pClonk = GetActionTarget();
  if(!ActIdle(pClonk)) pClonk->SetAction("Idle");
  if(alpha) Destructing();
  // Clonks im Eisblock sollen nicht frieren!
  LocalN("iWarmth", pClonk) = pClonk->MaximumWarmth();
  pClonk->~SetColdWarning(255);
  // Clonk bekommt Atem und Lebensenergie
  DoEnergy(100, pClonk);
  DoBreath(100, pClonk);
/*  // Magische Schriftrolle muss verwendet werden!
  // Eisblock wird auch aufgetaut, wenn die Angreifer in die Nähe kommen
  var pObj;
  while(pObj=FindObject(0,-20,-20,40,40,OCF_CrewMember | OCF_Alive,,,,pObj))
    if(GetOwner(pObj) >= 0)
      if(Global(GetOwner(pObj)) != Position_Castle)
        if(ScriptCounter() < 3395) 
          { goto(3395); break; }
*/
  return(1);
}

protected func Destruction()
{
  var pClonk = GetActionTarget();
  pClonk->SetAction("Walk");
  pClonk->SetObjectLayer(0);
}

private func Destructing()
{
  SetClrModulation(RGBa(255,255,255,alpha));
  alpha+=1;
  if(alpha>210) RemoveObject();
}

