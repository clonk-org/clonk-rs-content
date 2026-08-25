/*-- Realistische Bäume --*/

#strict                

protected func Initialize()
{
  ScheduleCall(this(), "CheckTrees", 1);
}

private func CheckTrees()
{
  var pObj;
  while(pObj = FindObject(,,,,,,,,, pObj))
    if(pObj->~IsTree())
      if(!GetEffectCount("IntRealisticWoods_fx6C", pObj))
      {
        AddEffect("IntRealisticWoods_fx6C", pObj, 1);
        if(!Random(10)) SetCategory(GetCategory(pObj) | C4D_Foreground, pObj);
      }
}

