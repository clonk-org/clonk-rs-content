/* Schutz */

#strict 2

func Activate(pCaster,pRealcaster)
{    
  if(GetEffect("ProtectionPSpell",pClonk)) { RemoveObject(); return; }
  
  var pClonk = pRealcaster;
  if(!pClonk) pClonk = pCaster;
  
  if(GetEffect("ProtectionPSpell",pClonk))
  {
   RemoveObject();
   return 1;
  }
  
  AddEffect("ProtectionPSpell",pClonk,250,2,0,GetID());
  RemoveObject();
  return 1;
}

func FxProtectionPSpellStart(pTarget, iEffectNumber, iEffectTime)
{
  EffectVar(0,pTarget,iEffectNumber) = OnFire(pTarget);
}

func FxProtectionPSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(!pTarget || !(pTarget->GetKarmaEnergy())) return -1;
  if(iEffectTime > 35*35) return -1;
  
  if(EffectVar(0,pTarget,iEffectNumber) && !OnFire(pTarget))
    EffectVar(0,pTarget,iEffectNumber)=0;

  for(var cnt=0; cnt<360; cnt+=90)
  {
    CreateParticle("PSpark",GetX(pTarget)+Cos(cnt+iEffectTime*2%360,16-3),GetY(pTarget)+Sin(cnt+iEffectTime*2%360,3),0,0,85,RGBa(255-Random(100),255-Random(100),50+Random(100),150),pTarget,(Sin(cnt+iEffectTime*2%360,3)<0));
    CreateParticle("PSpark",GetX(pTarget)+Cos(cnt+90+iEffectTime*4%360,3),GetY(pTarget)+Sin(cnt+90+iEffectTime*4%360,20-3),0,0,85,RGBa(255-Random(100),255-Random(100),50+Random(100),150),pTarget,(Sin(cnt+90+iEffectTime*4%360,20-3)<0));
  }

  return 1;
}

func FxProtectionPSpellDamage (object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  // Paar Sachen raus :]
  if(iCause == 40 || iCause == 38 || iCause == 37 || iCause == 36)
  return iDmgEngy;
 
  // Wehrt kurze Zeit alles ab und ist toll (nur wenn er schon vorher brannte nicht)
  if(iCause==4 && EffectVar(0,pTarget,iEffectNumber)) return iDmgEngy;
 
  // Mana abziehen
  var done=0;
  if(iDmgEngy<0)
  {
    for(;done<Abs(iDmgEngy)/2000;done++)
     if(!pTarget->DoKarmaEnergy(-3)) break;
  }

  return iDmgEngy+done*1500;
}

func FxProtectionPSpellEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  // Normales Feuer blocken, wenn nicht schon vorher gebrannt
  if(!EffectVar(0,pTarget,iEffectNumber))
    if(WildcardMatch(szNewEffectName, "*Fire*"))
      if(!WildcardMatch(szNewEffectName, "*Ban*") && !WildcardMatch(szNewEffectName, "*No*"))
      { if (pTarget) pTarget->DoKarmaEnergy(-6); return -1; }
}

func DoesMagic(pObj) { return WildcardMatch(GetAction(pObj), "*Magic"); }

protected func FxProtectionPSpellInfo() { return "{{MPRT}} $SpellInfo$"; }

// Ist ein Paladinwunder
public func IsWonder()   { return 1; }

// Nur für die Lichtkirche
//public func NeedChurch() { return scLightchurch; }

// Priorität, für Einordung ins Ringmenü
public func GetWonderPriority() { return 20; }

public func GetSpellClass(object pMage) { return(MISC); }
public func GetSpellCombo(pMage) { return ("111"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
