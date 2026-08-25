/*-- Holz statt Geld! --*/

#strict

global func StartWoodDisplay()
{
  AddEffect("IntWoodDisplay", 0, 1, 5);
}

global func FxIntWoodDisplayTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  for(var i=0; i<GetPlayerCount(); i++)
    SetWealth(GetPlayerByIndex(i), ObjectCount(WOOD,,,,,,,,,GetPlayerByIndex(i)));
}

