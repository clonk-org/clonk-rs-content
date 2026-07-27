/* Hitzeaura */

#strict 2

func Activate(pCaster,pRealcaster)
{
  var pClonk;
  // Hat er schon eine Aura, dann bedeutet nochmalzaubern Aura beenden
  if(GetEffect("HeatAura",pCaster)) { RemoveEffect("HeatAura",pCaster); return RemoveObject(); }
  var iEffect = AddEffect("HeatAura",pCaster,250,3,0,GetID());
  if(iEffect==-1) return 0;
  RemoveObject();
  return 1;
}

public func GetRange() { return 200; }

func FxHeatAuraTimer(pCaster, iEffectNumber, iEffectTime)
{
  // Zeit zu gehen	
  if(iEffectTime >= 600) return -1;
  var iRange = GetRange();
  var i=1;
  var pLive;
  var iCount;
  // Clonks mit Heilbann belegen
  for(var pLive in FindObjects(Find_Distance(iRange, GetX(pCaster), GetY(pCaster)),Find_Layer(GetObjectLayer()), Find_NoContainer(), Find_OCF(OCF_Alive)))
  {
    if(!Random(6)) DoEnergy(-1, pLive);
  }
  // Effekt machen
  CreateParticle("AuraFire",GetX(pCaster),GetY(pCaster), RandomX(-1,1),RandomX(-1,1), Abs(Sin(iEffectTime*10, 5*iRange)),RGB(255,130,0));
  CreateParticle("PSpark",GetX(pCaster),GetY(pCaster),0,0,10*iRange,RGB(16,0,0));
  return 1;
}

func FxHeatAuraStop(pCaster, iEffectNumber)
{
  return 1;
}

protected func FxHeatAuraInfo() { return "{{AUHE}} $SpellInfo$"; }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMFR; }
public func GetSpellStaffCombo(pMage) { return ("26"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

