/*-- Wärmen --*/

#strict

func Activate(pCaster,pRealcaster)
{
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
    if (!DoesMagic(pCaster)) return(0,RemoveObject());
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  // schon ganz gewärmt
  if(LocalN("iWarmth", pClonk)>=pClonk->~MaximumWarmth()-10) return(0,RemoveObject());
  AddEffect("WarmPSpell",pCaster,250,2,0,GetID(),pClonk);
  return(1,RemoveObject());
}

func FxWarmPSpellStart(pCaster, iEffectNumber, iTemp, pClonk)
{
  // Zielobjekt speichern: Das Zielobjekt des Zaubers ist nicht immer der
  // Zauberer selbst, Beispiel Zauberturm
  EffectVar(0,pCaster,iEffectNumber)=pClonk;
}

func FxWarmPSpellTimer(pCaster, iEffectNumber, iEffectTime)
{
  var pClonk=EffectVar(0,pCaster,iEffectNumber);
  var iMLength=GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));
  if(iEffectTime >= 200) return(-1);

  // magische Aktion vorhanden
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
  {
    // z.B. während des Zauberns durch einen Stein getroffen
    if(!DoesMagic(pCaster)) return(-1);
    // Zauberaktion für das zaubernde Objekt: Es wird angenommen, dass diese
    // in einem Raum von etwa 2/6 bis 5/6 der Zauberaktion spielt
    if(GetPhase(pCaster) >= iMLength*5/6) SetPhase(iMLength*2/6,pCaster);
  }

  // Abbruch wenn ganz gewärmt
  if(LocalN("iWarmth", pClonk)==pClonk->~MaximumWarmth()) return(-1);

  // Partikeleffekte für den betroffenen
  var clr = RGB(210,210-Random(50),20); if(!Random(3)) clr = RGB(210,20+Random(50),20);
  CreateParticle("MSpark",GetX(pClonk)+RandomX(-8, 8),GetY(pClonk)+RandomX(-10, 10),0,-20,RandomX(18,38),clr);
  CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,180,RGB(16,16,0),pCaster);
  // Clonk wärmen
  if(!Random(3)) LocalN("iWarmth", pClonk)=BoundBy(LocalN("iWarmth", pClonk)+60,0,pClonk->~MaximumWarmth());
  return(1);
}

func DoesMagic(pObj) { return(WildcardMatch(GetAction(pObj), "*Magic")); }

// Schlaegt im Zauberturm fehl, weil der die Magic-Aktion erst nach dem
// Ausfuehren des Zaubers setzt. Beizeiten mal irgendwie aendern.
func NoTowerMagic() { return(true); }

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("56"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
