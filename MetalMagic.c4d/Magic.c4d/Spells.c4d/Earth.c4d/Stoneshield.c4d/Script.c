/* Steinschild */

#strict 2

static const MSSH_OverlayLayer = 13;

local fRock;
func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  var pCombo;  
  if (pCombo = FindContents(ROCK, pCaster))
    {
    fRock = true;
    RemoveObject(pCombo);
    }
  
  // Auswählen wenn möglich
  if (pCaster->~DoSpellSelect(this,300)) return 1;

  // Clonk kann nicht auswählen: Clonk bekommst selbst ein Schild
  if (SelectorTarget(pClonk))
   return ActivateTarget(pClonk, pClonk);

  // ansonsten geth nich
  RemoveObject();
  return 0;
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  AddEffect("StoneShieldPSpell",pTarget,250,4,0,GetID(), fRock);
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
  // darf noch kein Schild haben
  if(GetEffect("StoneShieldPSpell", pTarget)) return;
  return 1;
}

func FxStoneShieldPSpellStart(pClonk, iEffectNumber, iTemp, fRock)
{
  if(iTemp) return;
  pClonk->SetGraphics(0, 0, STSH, MSSH_OverlayLayer, GFXOV_MODE_Base, 0, 0);
  pClonk->SetObjDrawTransform(1000,0,0, 0,1000,2000, 0, MSSH_OverlayLayer);
  if(fRock) EffectVar(0,pClonk,iEffectNumber)+=50000;
  else EffectVar(0,pClonk,iEffectNumber)+=30000;
}

func FxStoneShieldPSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
  if(!EffectVar(0,pClonk,iEffectNumber)) return -1;
  if(!Random(120)) EffectVar(0,pClonk,iEffectNumber)-=1000;
  CastParticles("PSpark", 1, 10, GetX(pClonk), GetY(pClonk)+4, 50, 70, RGBa(255, 200, 200, 50), RGBa(100, 100, 200, 75), pClonk);
  return 1;
}

func FxStoneShieldPSpellDamage(pClonk, iEffectNumber, iDmgEngy, iCause)
{
  if(iDmgEngy>0) return iDmgEngy;
  var iRestDmg = iDmgEngy/5;
  iDmgEngy = -iDmgEngy+iDmgEngy/5;
  var i = iDmgEngy;
  if(EffectVar(0,pClonk,iEffectNumber)>iDmgEngy)
  {
    EffectVar(0,pClonk,iEffectNumber)-=iDmgEngy;
    iDmgEngy = 0;
  }
  else
  {
    iDmgEngy-=EffectVar(0,pClonk,iEffectNumber);
    EffectVar(0,pClonk,iEffectNumber)=0;
  }
  CastParticles("PSpark", (i-iDmgEngy)/500, 50, GetX(pClonk), GetY(pClonk), 50, 70, RGBa(255, 200, 200, 50), RGBa(200, 200, 200, 75));
  return -iDmgEngy+iRestDmg;
}

func FxStoneShieldPSpellStop(pClonk, iEffectNumber, iReason, iTemp)
{
  if(iTemp) return;	
  SetGraphics(0, pClonk, 0, MSSH_OverlayLayer);
}

private func FxStoneShieldPSpellEffect(string szNewEffectName, object pTarget, int iNumber) 
{ 
  if(szNewEffectName == "StoneShieldPSpell")
  {
    if(EffectVar(0,pTarget,iNumber)>50000) return -1;
    return -2;
  }
}

private func FxStoneShieldPSpellAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += 30000;
  if(EffectVar(0,pTarget,iNumber)> 60000) EffectVar(0,pTarget,iNumber)=60000;
}

protected func FxStoneShieldPSpellInfo() { return "{{MSSH}} $SpellInfo$"; }

/* Zaubercombo */

public func GetSpellStaff() { return SMER; }
public func GetSpellStaffCombo() { return ("56"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

public func GetSpellClass() { return EART; }
public func GetSpellCombo() { return ("122"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
