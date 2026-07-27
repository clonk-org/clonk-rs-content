/* Zurückwerfen */

#strict 2

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  
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
  // Effekt geben
  AddEffect("DeflectionPSpell",pTarget,250,2,0,GetID());
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
  // darf noch keinen Effekt haben
  if(GetEffect("DeflectionPSpell", pTarget)) return;
  return 1;
}

func FxDeflectionPSpellStart(pClonk, iEffectNumber, iTemp)
{
  if(iTemp) return;
  // Zeit setzen
  EffectVar(0,pClonk,iEffectNumber)+=450;
}

func FxDeflectionPSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
  // Zeit um
  if(!EffectVar(0,pClonk,iEffectNumber)--) return -1;
  // Nicht in Gebäuden
  if(Contained(pClonk)) return;
  //Funken an der benötigten Stelle
  for(var pObj in FindObjects(Find_OCF(OCF_HitSpeed1),Find_Layer(GetObjectLayer()), Find_NoContainer(), Find_Category(C4D_Object), Find_Distance(21, GetX(pClonk), GetY(pClonk)), Find_Not(Find_Distance(11, GetX(pClonk), GetY(pClonk)))))
  {
    // Wenn das Objekt auf den Clonk zufliegt
    if( (GetXDir(pObj)<0) == (GetX(pObj)>GetX(pClonk)) || (GetYDir(pObj)<0) == (GetY(pObj)>GetY(pClonk)))
    {
      // Eigenen Stab ignorieren
      if(GetEffect("StaffFlight", pObj))
        if(EffectVar(0, pObj, GetEffect("StaffFlight", pObj))==pClonk)
          continue;
      if(Abs(GetXDir(pObj))<5) SetXDir(GetXDir(pObj)+RandomX(-10,10), pObj);
      if(Abs(GetYDir(pObj))<5) SetYDir(3, pObj);
      SetSpeed(-GetXDir(pObj),-GetYDir(pObj), pObj);
      CreateParticle("NoGravSpark", AbsX(GetX(pObj)), AbsY(GetY(pObj)), 0, 0, 80, RGB(80,50,50));
      if(!GetEffect("IntDeflected", pObj))
        EffectVar(0,pClonk,iEffectNumber) = Max(EffectVar(0,pClonk,iEffectNumber)-50,0);
      AddEffect("IntDeflected", pObj, 1, 20);
      Sound("Rip", 0, pObj);
    }
  }
  // Partikel Kreiseffekt
  if(Mod(iEffectTime,4)) return;
  var iAngle = iEffectTime*10;
  while(iAngle>=360) iAngle -= 360;
  var px= Sin(iAngle, 15);
  var py= Cos(iAngle, 15);
  CreateParticle("NoGravSpark", px+GetX(pClonk), py+GetY(pClonk), 0, 0, 50, RGB(50,80,50), pClonk);
  CreateParticle("NoGravSpark",-py+GetX(pClonk), px+GetY(pClonk), 0, 0, 50, RGB(50,50,80), pClonk);
  CreateParticle("NoGravSpark",-px+GetX(pClonk),-py+GetY(pClonk), 0, 0, 50, RGB(50,80,50), pClonk);
  CreateParticle("NoGravSpark", py+GetX(pClonk),-px+GetY(pClonk), 0, 0, 50, RGB(50,50,80), pClonk);
  return 1;
}

private func FxDeflectionPSpellEffect(string szNewEffectName, object pTarget, int iNumber) 
{ 
  // Andere Effekte aufnehmen
  if(szNewEffectName == "DeflectionPSpell")
  {
    if(EffectVar(0,pTarget,iNumber)>50) return -1;
    return -2;
  }
}

private func FxDeflectionPSpellAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += 300;
  if(EffectVar(0,pTarget,iNumber)> 600) EffectVar(0,pTarget,iNumber)=600;
}

protected func FxDeflectionPSpellInfo() { return "{{MDFL}} $SpellInfo$"; }

/* Zaubercombo */

public func GetSpellStaff() { return SMAR; }
public func GetSpellStaffCombo() { return ("56"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
