/* Heilungsaura */

#strict 2

func Activate(pCaster,pRealcaster)
{
  AddEffect("HealAuraPSpell",pCaster,250,3,0,GetID());
  RemoveObject();
  return 1;
}

func FxHealAuraPSpellTimer(pCaster, iEffectNumber, iEffectTime)
{
  var iMLength=GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));
  if(iEffectTime >= 400) return -1;
  
  // Effekt machen
  var iRange = GetRange();
  CreateParticle("Aura",GetX(pCaster),GetY(pCaster), RandomX(-1,1),RandomX(-1,1), 6*GetRange() + Sin(iEffectTime*3, 3*32),RGB(200,255,0));
  for(var i = 0; i < 3; i++)
    CreateParticle("PxSpark",GetX(pCaster)+RandomX(iRange,-iRange)*2/5,GetY(pCaster)+RandomX(iRange,-iRange)*2/5,0,RandomX(-5,-10),RandomX(50,100),RGB(250-Random(100),20+Random(100),20));

  
  // alle Clonks in Reichweite heilen
  DoHeal(pCaster);
  return 1;
}

protected func FxHealAuraPSpellInfo() { return "{{MHGA}} $SpellInfo$"; }

func DoHeal(pCaster)
{
  var pClonk;
  for(pClonk in FindObjects(Find_Distance(GetRange(), GetX(pCaster), GetY(pCaster)),Find_Layer(GetObjectLayer(pCaster)), Find_NoContainer(), Find_OCF(OCF_CrewMember)))
      if(!Hostile(GetOwner(pClonk),GetOwner(pCaster)) || ObjectCount(NTMG))
      {
        // heilen
        if(!Random(4)) DoEnergy(+1,pClonk);
        // Partikeleffekte für jeden betroffenen:
        CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),RGB(210,20,20));
        CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,180,RGB(16,8,0),pClonk);
      }
}

public func GetRange() { return 200; }

// Wie bei Heilung: Versagt vorerst im eh im Zauberturm
func NoTowerMagic() { return true; }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMWT; }
public func GetSpellStaffCombo(pMage) { return ("22"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
