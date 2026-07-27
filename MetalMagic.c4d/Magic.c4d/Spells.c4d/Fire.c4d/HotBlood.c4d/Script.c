/* Heißes Blut */

#strict 2

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  // Auswählen wenn möglich
  if (pCaster->~DoSpellSelect(this,300)) return 1;

  // Clonk kann nicht auswählen: Clonk bekommst selbst den Bonus
  if (SelectorTarget(pClonk))
   return ActivateTarget(pClonk, pClonk);

  // ansonsten geht nicht
  RemoveObject();
  return 0;
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  AddEffect("HotBloodPSpell",pTarget,250,4,0,GetID());
  // Objekt löschen
  return RemoveObject();
}

public func SelectorTarget(object pTarget)
{
  // nur außerhalb von Gebäuden
  if(Contained(pTarget)) return;
  // muss ein Mannschaftsmitglied sein
  if(!(GetOCF(pTarget)&OCF_CrewMember)) return;
  // muss feindlich sein oder NTMG muss aktiviert sein
  if(Hostile(GetOwner(),GetOwner(pTarget)) && !ObjectCount(NTMG)) return;
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return;
  // darf noch kein "Heißes Blut" haben
  if(GetEffect("HotBloodPSpell", pTarget)) return;
  return 1;
}

func FxHotBloodPSpellStart(pClonk, iEffectNumber, iTemp)
{
  if(iTemp) return;
  if(OnFire(pClonk))
  {
    EffectVar(0,pClonk,iEffectNumber)+=50;
    EffectVar(1,pClonk,iEffectNumber) = 1;
  }
  else EffectVar(0,pClonk,iEffectNumber)+=30;
  Incinerate(pClonk);
}

func FxHotBloodPSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
  if(!EffectVar(0,pClonk,iEffectNumber)--) return -1;
  if(!OnFire(pClonk)) return -1;
  return 1;
}

func FxHotBloodPSpellDamage(pClonk, iEffectNumber, iDmgEngy, iCause)
{
  if(iDmgEngy>0) return iDmgEngy;
  if(iCause == 35) return 0;
  return iDmgEngy;
}

func FxHotBloodPSpellStop(pClonk, iEffectNumber, iReason, iTemp)
{
  if(iTemp) return;
  if(!EffectVar(1,pClonk,iEffectNumber)) Extinguish(pClonk);
}

private func FxHotBloodPSpellEffect(string szNewEffectName, object pTarget, int iNumber) 
{ 
  if(szNewEffectName == "HotBloodPSpell")
  {
    if(EffectVar(0,pTarget,iNumber)>50) return -1;
    return -2;
  }
}

private func FxHotBloodPSpellAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += 30;
  if(EffectVar(0,pTarget,iNumber)> 60) EffectVar(0,pTarget,iNumber)=60;
}

protected func FxHotBloodPSpellInfo() { return "{{MHTB}} $SpellInfo$"; }

/* Zaubercombo */

public func GetSpellStaff() { return SMFR; }
public func GetSpellStaffCombo() { return ("56"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
