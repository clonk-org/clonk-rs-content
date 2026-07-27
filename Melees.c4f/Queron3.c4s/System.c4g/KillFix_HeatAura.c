/* Hitzeaura: Killverfolgung und weniger Reichweite */

#strict 2
#appendto AUHE

//Kleinere Reichweite, Killverfolgung

public func GetRange() {return 150;}

func FxHeatAuraTimer(pCaster, iEffectNumber, iEffectTime)
{
  // Zeit zu gehen	
  if(iEffectTime >= 600) return -1;
  var iRange = GetRange();

  for(var pLive in FindObjects(Find_Distance(iRange, GetX(pCaster), GetY(pCaster)),Find_Layer(GetObjectLayer()), Find_NoContainer(), Find_OCF(OCF_Alive)))
  {
    if(!Random(6)) DoEnergy(-1, pLive, false, 0, GetOwner(pCaster) + 1);
  }
  // Effekt machen
  CreateParticle("AuraFire",GetX(pCaster),GetY(pCaster), RandomX(-1,1),RandomX(-1,1), Abs(Sin(iEffectTime*10, 5*iRange)),RGB(255,130,0));
  CreateParticle("PSpark",GetX(pCaster),GetY(pCaster),0,0,10*iRange,RGB(16,0,0));
  return 1;
}
