/* Gruppenheilung */

#strict

func Activate(pCaster,pRealcaster)
{
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
    if(GetAction(pCaster) ne "Magic") return(0,RemoveObject());
  AddEffect("GroupWarmPSpell",pCaster,250,2,0,GetID());
  return(1,RemoveObject());
}

func FxGroupWarmPSpellTimer(pCaster, iEffectNumber, iEffectTime)
{
  var iMLength=GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));
  if(iEffectTime >= 300) return(-1);

  // magische Aktion vorhanden
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
  {
    if(!DoesMagic(pCaster)) return(-1);
    // Zauberaktion für den Magier
    if(GetPhase(pCaster) >= iMLength*5/6) SetPhase(iMLength*2/6,pCaster);
  }
  
  // Effekt machen
  CreateParticle("Aura2",GetX(pCaster),GetY(pCaster), RandomX(-1,1),RandomX(-1,1), 5*160 + Sin(iEffectTime*3, 5*32),RGB(200,255,0));
  
  // alle Clonks in Reichweite heilen
  DoWarm(pCaster);
  return(1);
}

func DoWarm(pCaster)
{
  var pClonk;
  while(pClonk=FindObject(0,GetX(pCaster)-80,GetY(pCaster)-80,GetX(pCaster),GetY(pCaster),OCF_CrewMember(),0,0,NoContainer(),pClonk))
    if(ObjectDistance(pClonk,pCaster) <= 80)
      if(!Hostile(GetOwner(pClonk),GetOwner(pCaster)) || ObjectCount(NTMG))
      {
        // wärmen
        if(!Random(3)) LocalN("iWarmth", pClonk)=BoundBy(LocalN("iWarmth", pClonk)+50,0,pClonk->~MaximumWarmth());
        // Partikeleffekte für jeden betroffenen:
        var clr = RGB(210,210-Random(50),20); if(!Random(3)) clr = RGB(210,20+Random(50),20);
        CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),clr);
        CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,180,RGB(16,16,0),pClonk);
      }
}

func DoesMagic(pObj) { return(WildcardMatch(GetAction(pObj), "*Magic")); }

// Wie bei Heilung: Versagt vorerst im Zauberturm
func NoTowerMagic() { return(true); }

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("52"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
