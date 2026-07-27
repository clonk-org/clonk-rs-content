/* PositionxChange! */

#strict 2
local pSorc;
func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  // Auswählen wenn möglich
  pSorc=pClonk;
  if (pCaster->~DoSpellSelect(this,500)) return 1;

  // Clonk kann nicht auswählen:
  // Bei diesem Zauber leider versagt
  Message("$NoTarget$", pClonk);
  RemoveObject();
  return 0;
}

public func ActivateTarget(object pCaller, object pTarget)
{
  // Typ heilen
  AddEffect("PositionExchangeSpell",pCaller,250,4,0,GetID(),pTarget);
  // Objekt löschen
  return RemoveObject();
}

public func SelectorTarget(object pTarget)
{
  // nur außerhalb von Gebäuden
  if(Contained(pTarget)) return;
  // darf nicht der Clonk selber sein
  if(pTarget == pSorc) return;
  // muss ein Mannschaftsmitglied sein
  if(!(GetOCF(pTarget)&OCF_CrewMember)) return;
  // Muss auch zu einem Spieler gehören
  if(GetOwner(pTarget) == -1)return 0;
  // Darf nicht irgendwo hinter sein
  if(!PathFree(GetX(pTarget),GetY(pTarget),GetX(pSorc),GetY(pSorc)))
  {
    // Sonst geringere Reichweite
    if(ObjectDistance(pTarget, pSorc)>150) return 0;
    // Und muss überhaupt zugänglich sein
    if(!GetPathLength(GetX(pTarget),GetY(pTarget),GetX(pSorc),GetY(pSorc))) return 0;
  }
  // darf nicht feindlich sein oder NTMG muss aktiviert sein <- darf wohl?
  if(Hostile(GetOwner(),GetOwner(pTarget)) && !ObjectCount(NTMG)) return;
  // Wird noch nicht gedingstauscht
  if (GetEffect("PositionExchangeBlock",pTarget) || GetEffect("PositionExchangeSpell",pTarget)) return; 
  // sichtbar
  if(!Visible(GetOwner(),pTarget)) return;
  return 1;
}

public func FxPositionExchangeBlockStart()
{return 1;}

public func FxPositionExchangeBlockTimer()
{return -1;}

func FxPositionExchangeSpellStart(pTarget, iEffectNumber, iTemp, par1)
{
  EffectVar(0,pTarget,iEffectNumber)=par1;
  AddEffect("PositionExchangeBlock",par1,20,90,0,MCIB);
  return 1;
}

func FxPositionExchangeSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
  
  var pOther=EffectVar(0,pTarget,iEffectNumber);
  
  if(iEffectTime <= 30)return 0;
  
  if(iEffectTime > 30 && iEffectTime < 90)
  {
   CastParticles("FSpark",3,5,GetX(pTarget),GetY(pTarget),40,90,RGB(10,10,10),RGB(255,255,255),pTarget);
   CastParticles("FSpark",3,5,GetX(pOther),GetY(pOther),40,90,RGB(10,10,10),RGB(255,255,255),pOther);
   return 1;
  }
  
  Sound("Plopp",0,pTarget,200);
  Sound("Plopp",0,pOther,200);
  
  var x=GetX(pTarget);
  var y=GetY(pTarget);
  
  if(pTarget != pOther && pOther)
  {
  SetPosition(GetX(pOther),GetY(pOther),pTarget);
  SetPosition(x,y,pOther);
  }
  
 if(GetEffect("PositionExchangeBlock",EffectVar(0,pTarget,iEffectNumber)))
 {
 RemoveEffect("PositionExchangeBlock",EffectVar(0,pTarget,iEffectNumber),
 GetEffect("PositionExchangeBlock",EffectVar(0,pTarget,iEffectNumber)));
 
 }
  return -1;
}

func FxPositionExchangeSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
}

func NoTowerMagic() { return true; }


public func IsBattlemageSpell() { return 1; }

/* Zaubercombo */

public func GetSpellStaff(object pMage) { return SMER; }
public func GetSpellStaffCombo() { return ("155"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
